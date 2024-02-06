#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Color.h"

TEST(ColorTests, Arithmetic){
    Color a(0.9,0.6,0.75);
    Color b(0.7, 0.1, 0.25);
    EXPECT_EQ(a.get_resolution(),b.get_resolution());
    EXPECT_EQ(a.get_dim(),b.get_dim());
    Color added(1.6,0.7,1.0);
    EXPECT_EQ(a+b, added);
    Color subtracted(0.2,0.5,0.5);
    EXPECT_EQ(a-b, subtracted);
    Color Init(0.2,0.3,0.4);
    Color ScalarMul(0.4, 0.6, 0.8);
    EXPECT_EQ(2*Init,ScalarMul);
    Color ColorMul(0.63, 0.06, 0.1875);
    EXPECT_EQ(a*b,ColorMul);
}