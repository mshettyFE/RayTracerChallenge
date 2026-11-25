#include <gtest/gtest.h>
#include <cmath>
#include "TerminalDisplay.h"

static const std::string HALF_BLOCK = "\xe2\x96\x80";

// Each cell is the upper-half block with fg = top pixel and bg = bottom pixel. A colour code is
// only emitted when it differs from the previous cell in the row.
TEST(TerminalDisplay, EncodesPixelPairsAndSkipsRepeatedColours){
    Canvas c(2, 2);
    c.write_pixel(0, 0, RED);  c.write_pixel(1, 0, RED);
    c.write_pixel(0, 1, BLUE); c.write_pixel(1, 1, GREEN);
    const std::string expected =
        "\x1b[H"
        "\x1b[38;2;255;0;0m" "\x1b[48;2;0;0;255m" + HALF_BLOCK +
        /* fg still red: no code */ "\x1b[48;2;0;255;0m" + HALF_BLOCK +
        "\x1b[0m\r\n"
        "\x1b[0m" "ok" "\x1b[K";
    EXPECT_EQ(TerminalDisplay::encode_frame(c, "ok"), expected);
}

TEST(TerminalDisplay, OddHeightPadsBottomWithBlack){
    Canvas c(1, 1);
    c.write_pixel(0, 0, WHITE);
    const std::string frame = TerminalDisplay::encode_frame(c, "");
    EXPECT_NE(frame.find("\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m" + HALF_BLOCK), std::string::npos);
}

TEST(TerminalDisplay, StatusLineIsCutToImageWidth){
    Canvas c(3, 2);
    const std::string frame = TerminalDisplay::encode_frame(c, "abcdef");
    EXPECT_NE(frame.find("\x1b[0mabc\x1b[K"), std::string::npos);
    EXPECT_EQ(frame.find("abcd"), std::string::npos);
}

TEST(TerminalDisplay, ColoursAreClampedTo8Bit){
    EXPECT_EQ(Color(1.7, -0.3, 0.5).to_rgb8(), (std::array<unsigned char,3>{255, 0, 127}));
    EXPECT_EQ(Color(NAN, 0, 0).to_rgb8()[0], 0);
}
