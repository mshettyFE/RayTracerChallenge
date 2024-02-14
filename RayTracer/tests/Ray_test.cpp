#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Tuple.h"

TEST(RayTest,Init){
    Ray a(Tuple({1,0,0},TupType::POINT), Tuple({0,1,0},TupType::VECTOR));
    Tuple origin({1,0,0}, TupType::POINT);
    Tuple dir({0,1,0}, TupType::VECTOR);
    Ray b(origin, dir);
    EXPECT_EQ(a,b);
}

TEST(RayTest,math){
    Ray a(Tuple({2,3,4}, TupType::POINT), Tuple({1,0,0}, TupType::VECTOR));
    EXPECT_EQ(a.position(0), Tuple({2,3,4}, TupType::POINT));
    EXPECT_EQ(a.position(1), Tuple({3,3,4}, TupType::POINT));
    EXPECT_EQ(a.position(-1), Tuple({1,3,4}, TupType::POINT));
    EXPECT_EQ(a.position(2.5), Tuple({4.5,3,4}, TupType::POINT));
}