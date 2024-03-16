#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "SmoothTriangle.h"

TEST(SmoothTriangleTest, Intersect){
    SmoothTriangle t({0, 1, 0}, {-1, 0, 0}, {1, 0, 0},{0,1,0},{-1,0,0},{1,0,0});
    Ray r({-0.2, 0.3, -2}, {0, 0, 1});
    auto i = t.intersect(r);
    EXPECT_EQ(i.size(), 1);
    EXPECT_DOUBLE_EQ(i[0].get_u(), 0.45);
    EXPECT_DOUBLE_EQ(i[0].get_v(), 0.25);
}

TEST(SmoothTriangleTest, Normal){
    SmoothTriangle t({0, 1, 0}, {-1, 0, 0}, {1, 0, 0},{0,1,0},{-1,0,0},{1,0,0});
    Impact i = Impact(1,&t,0.45,0.25);
    EXPECT_EQ(t.normal_at(GenPoint(0,0,0),i),GenVec(-0.5547, 0.83205, 0));
}
