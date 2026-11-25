#include "TerminalDisplay.h"
#include <array>
#include <cerrno>
#include <csignal>
#include <stdexcept>
#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace {

// (re: std::signal needs free function, so information about what to set the terminal 
// settings to needs to be outside of class. unnamed namespace keeps things tidy)
termios original_settings;
volatile sig_atomic_t active = 0;

// Reset colours, show the cursor, leave the alternate screen.
const char RESTORE_SEQUENCE[] = "\x1b[0m\x1b[?25h\x1b[?1049l";
// Enter the alternate screen, hide the cursor, clear it.
const char ENTER_SEQUENCE[] = "\x1b[?1049h\x1b[?25l\x1b[2J";

void write_all(const char* data, size_t len){
    while(len > 0){
      // NB: :: to reference global namespace for POSIX calls. Defensive
        ssize_t n = ::write(STDOUT_FILENO, data, len);
        if(n < 0){
            if(errno == EINTR){continue;}
            return;
        }
        data += n;
        len -= static_cast<size_t>(n);
    }
}

// Only async-signal-safe calls (write, tcsetattr), since this also runs from the signal handler.
void restore_terminal(){
    if(!active){return;}
    active = 0;
    write_all(RESTORE_SEQUENCE, sizeof(RESTORE_SEQUENCE) - 1);
    ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_settings);
}

void on_signal(int sig){
    restore_terminal();
    // Restore default interrupt
    std::signal(sig, SIG_DFL);
    // Execute interrupt
    std::raise(sig);   
}

// Escape sequences used to draw a frame.
const char CURSOR_HOME[] = "\x1b[H";
const char FG_PREFIX[] = "\x1b[38;2;";      // followed by "r;g;bm"
const char BG_PREFIX[] = "\x1b[48;2;";
const char RESET_ATTRIBUTES[] = "\x1b[0m";
const char ROW_END[] = "\x1b[0m\r\n";       // reset first, so the background doesn't bleed past the row
const char CLEAR_TO_LINE_END[] = "\x1b[K";
const char HALF_BLOCK[] = "\xe2\x96\x80";   // U+2580 UPPER HALF BLOCK, in UTF-8

// Length of a string literal, without its terminating '\0'.
template <size_t N>
constexpr size_t literal_length(const char (&)[N]){return N - 1;}

// Worst case bytes for one cell: a new foreground and background colour (each at most
// prefix + "255;255;255" + 'm'), then the glyph.
constexpr size_t MAX_COLOUR_CODE_BYTES = literal_length(FG_PREFIX) + literal_length("255;255;255") + 1;
constexpr size_t MAX_CELL_BYTES = 2 * MAX_COLOUR_CODE_BYTES + literal_length(HALF_BLOCK);

void append_colour(std::string& out, const char* prefix, const std::array<unsigned char,3>& rgb){
    out += prefix;
    out += std::to_string(rgb[0]);
    out += ';';
    out += std::to_string(rgb[1]);
    out += ';';
    out += std::to_string(rgb[2]);
    out += 'm';
}

} // namespace

TerminalDisplay::TerminalDisplay(){
    if(!::isatty(STDIN_FILENO) || !::isatty(STDOUT_FILENO)){
        throw std::runtime_error("TerminalDisplay needs stdin and stdout to be a terminal");
    }
    if(::tcgetattr(STDIN_FILENO, &original_settings) != 0){
        throw std::runtime_error("Could not read terminal settings");
    }
    termios raw = original_settings;
    // No line buffering, no echo, and Ctrl-C/Ctrl-Z arrive as bytes rather than signals, so
    // the program can't be suspended with the terminal left in raw mode.
    raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;    // read() returns immediately, with whatever is available
    raw.c_cc[VTIME] = 0;
    if(::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0){
        throw std::runtime_error("Could not put terminal in raw mode");
    }
    active = 1;
    std::signal(SIGTERM, on_signal);
    std::signal(SIGHUP, on_signal);
    std::signal(SIGINT, on_signal);
    write_all(ENTER_SEQUENCE, sizeof(ENTER_SEQUENCE) - 1);
}

TerminalDisplay::~TerminalDisplay(){
    restore_terminal();
    std::signal(SIGTERM, SIG_DFL);
    std::signal(SIGHUP, SIG_DFL);
    std::signal(SIGINT, SIG_DFL);
}

TerminalDisplay::Size TerminalDisplay::size() const{
    winsize ws{};
    if(::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_col == 0 || ws.ws_row == 0){
        return {80, 24};
    }
    return {ws.ws_col, ws.ws_row};
}

std::string TerminalDisplay::read_keys(int timeout_ms){
    pollfd pfd{STDIN_FILENO, POLLIN, 0};
    std::string keys;
    if(::poll(&pfd, 1, timeout_ms) <= 0){return keys;}
    char buf[64];
    ssize_t n;
    while((n = ::read(STDIN_FILENO, buf, sizeof(buf))) > 0){
        keys.append(buf, static_cast<size_t>(n));
    }
    return keys;
}

void TerminalDisplay::draw(const Canvas& image, const std::string& status){
    const std::string frame = encode_frame(image, status);
    write_all(frame.data(), frame.size());
}

std::string TerminalDisplay::encode_frame(const Canvas& image, const std::string& status){
    const unsigned int width = image.get_width();
    const unsigned int height = image.get_height();
    std::string out;
    const size_t rows = (height + 1) / 2;   // two pixels per character cell
    out.reserve(literal_length(CURSOR_HOME)
                + rows * (width * MAX_CELL_BYTES + literal_length(ROW_END))
                + literal_length(RESET_ATTRIBUTES) + width + literal_length(CLEAR_TO_LINE_END));
    out += CURSOR_HOME;   // overwrite in place rather than clearing the screen (no flicker)

    // Colour codes are only emitted when they change from the previous cell, which shrinks
    // the frame a lot for smooth regions like the sky and floor.
    const std::array<unsigned char,3> black{0, 0, 0};
    for(unsigned int y = 0; y < height; y += 2){
        bool have_fg = false, have_bg = false;
        std::array<unsigned char,3> fg{}, bg{};
        for(unsigned int x = 0; x < width; ++x){
            const auto top = image.pixel_at(x, y).to_rgb8();
            const auto bottom = (y + 1 < height) ? image.pixel_at(x, y + 1).to_rgb8() : black;
            if(!have_fg || top != fg){
              append_colour(out, FG_PREFIX, top);
              fg = top;
              have_fg = true;
            }
            if(!have_bg || bottom != bg){
              append_colour(out, BG_PREFIX, bottom);
              bg = bottom;
              have_bg = true;
            }
            out += HALF_BLOCK;
        }
        out += ROW_END;
    }

    // Status line, cut to the image width, clearing whatever was there before. No trailing
    // newline: on the last terminal row that would scroll the whole display up.
    out += RESET_ATTRIBUTES;
    out += status.substr(0, width);
    out += CLEAR_TO_LINE_END;
    return out;
}
