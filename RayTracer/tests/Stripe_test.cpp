#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Stripes.h"
#include "Canvas.h"

TEST(StripeTest,ProperSetup){
    Stripes s(WHITE, BLACK);
    EXPECT_EQ(s.at(Tuple({0,0,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,1,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,2,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,0,1},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,0,2},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0.9,0,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-0.1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-1.1,0,0},TupType::POINT)), BLACK);
}