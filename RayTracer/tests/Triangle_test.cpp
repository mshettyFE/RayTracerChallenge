#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Triangle.h"
#include "Canvas.h"

TEST(TriangleTest, Initialize){
    Triangle t({0,1,0},{-1,0,0}, {1,0,0});
    auto pts=  t.get_points();
    EXPECT_EQ(pts[0].type(), TupType::POINT);
    EXPECT_EQ(pts[0],Tuple({0,1,0},TupType::POINT));
    EXPECT_EQ(pts[1].type(), TupType::POINT);
    EXPECT_EQ(pts[1],Tuple({-1,0,0},TupType::POINT));
    EXPECT_EQ(pts[2].type(), TupType::POINT);
    EXPECT_EQ(pts[2],Tuple({1,0,0},TupType::POINT));
    EXPECT_EQ(t.get_edge1(), Tuple({-1,-1,0}));
    EXPECT_EQ(t.get_edge2(), Tuple({1,-1,0}));
    EXPECT_EQ(t.normal_at(pts[0]), Tuple({0,0,-1}));
}

TEST(TriangleTest, Normals){
    Triangle t({0,1,0},{-1,0,0}, {1,0,0});
    Tuple n1 = t.normal_at(Tuple({0, 0.5, 0}, TupType::POINT));
    Tuple n2 = t.normal_at(Tuple({-0.5, 0.75, 0}, TupType::POINT));
    Tuple n3 = t.normal_at(Tuple({0.5, 0.25, 0}, TupType::POINT));
    EXPECT_EQ(n1,n2);
    EXPECT_EQ(n2,n3);
    EXPECT_EQ(n3,n1);
}

TEST(TriangleTest, Intersection){
    Triangle t({0,1,0},{-1,0,0}, {1,0,0});
    Ray r({0,-1,2},{0,1,0});
    t.intersect(r);
}