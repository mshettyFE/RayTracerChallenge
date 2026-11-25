#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include "Canvas.h"
#include <string>

// Uses the terminal as a display. While a TerminalDisplay exists, the terminal is in raw mode
// (keys arrive immediately, unechoed), with the cursor hidden.
// The destructor restores it, and so does a handler for SIGTERM/SIGHUP/SIGINT.
//
// Images are drawn with the upper-half-block character: each character cell shows two
// vertically stacked pixels (foreground = top, background = bottom)
// Needed because terminal "pixels" are twice as tall as the are wide 

class TerminalDisplay{
public:
    struct Size{
        int cols;
        int rows;
        bool operator==(const Size& o) const{return cols == o.cols && rows == o.rows;}
        bool operator!=(const Size& o) const{return !(*this == o);}
    };

    // Throws std::runtime_error if stdin or stdout is not a terminal.
    TerminalDisplay();
    ~TerminalDisplay();
    TerminalDisplay(const TerminalDisplay&) = delete;
    TerminalDisplay& operator=(const TerminalDisplay&) = delete;

    // Current terminal size in character cells.
    Size size() const;

    // Waits up to timeout_ms for input, then returns every byte that is pending (possibly none).
    std::string read_keys(int timeout_ms);

    // Draws image starting at the top-left cell, followed by one status line.
    // image should be cols wide and 2*(rows-1) tall to fill the terminal exactly.
    void draw(const Canvas& image, const std::string& status);

    // The exact bytes draw() writes
    static std::string encode_frame(const Canvas& image, const std::string& status);
};

#endif
