#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Color.h"

TEST(ColorTests, Arithmetic){
    std::cout << "Start" << std::endl;
    Color a(0.9,0.6,0.75);
    Color b(0.7, 0.1, 0.25);
    Color added(1.6,0.7,1.0);
    EXPECT_EQ(a+b, added);
    Color subtracted(0.2,0.5,0.5);
    EXPECT_EQ(a-b, subtracted);
    Color ScalarMul(1.8,1.2,1.5);
    EXPECT_EQ(2*a,ScalarMul);
    Color ColorMul(0.9,0.2,0.04);
    EXPECT_EQ(a*b, ColorMul);
}