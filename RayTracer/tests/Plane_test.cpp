#include "Constants.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "PointSource.h"
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>

TEST(PlaneTest,IntersectionAbove){
    Plane p = Plane();
    Ray r = Ray(Tuple({0,1,0}, TupType::POINT),Tuple({0,-1,0}, TupType::VECTOR));
    std::vector<Impact> hits = p.intersect(r);
    EXPECT_EQ(hits.size(),1);
    EXPECT_EQ(hits[0].get_t(),1);
}

TEST(PlaneTest,IntersectionBelow){
    Plane p = Plane();
    Ray r = Ray(Tuple({0,-1,0}, TupType::POINT),Tuple({0,1,0}, TupType::VECTOR));
    std::vector<Impact> hits = p.intersect(r);
    EXPECT_EQ(hits.size(),1);
    EXPECT_EQ(hits[0].get_t(),1);
}