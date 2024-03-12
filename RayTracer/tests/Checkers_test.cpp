#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Checkers.h"

TEST(CheckersTest, Init){
    auto pattern = Checkers(WHITE,BLACK);
    EXPECT_EQ(pattern.at(Tuple({0,0,0}, TupType::POINT)),WHITE);
    EXPECT_EQ(pattern.at(Tuple({0.99,0,0}, TupType::POINT)),WHITE);
    EXPECT_EQ(pattern.at(Tuple({1.01,0,0}, TupType::POINT)),BLACK);

    EXPECT_EQ(pattern.at(Tuple({0,0.99,0}, TupType::POINT)),WHITE);
    EXPECT_EQ(pattern.at(Tuple({0,1.01,0}, TupType::POINT)),BLACK);

    EXPECT_EQ(pattern.at(Tuple({0,0,0}, TupType::POINT)),WHITE);
    EXPECT_EQ(pattern.at(Tuple({0,0,0.99}, TupType::POINT)),WHITE);
    EXPECT_EQ(pattern.at(Tuple({0,0,1.01}, TupType::POINT)),BLACK);
}