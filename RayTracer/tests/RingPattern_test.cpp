#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "RingPattern.h"

TEST(RingPatternTest, Check){
    auto pat = RingPattern(WHITE,BLACK);
    EXPECT_EQ(pat.at(Tuple({0,0,0},TupType::POINT)), WHITE);
    EXPECT_EQ(pat.at(Tuple({1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(pat.at(Tuple({0,0,1},TupType::POINT)), BLACK);
    EXPECT_EQ(pat.at(Tuple({0.708, 0, 0.708},TupType::POINT)), BLACK);
}