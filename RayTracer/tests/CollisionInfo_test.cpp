#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tuple.h"
#include "Plane.h"

TEST(CollisionInfoTest, CollisionInfoInit){
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::shared_ptr<Sphere> s = std::make_shared<Sphere>(Sphere());
    Impact hit(4,s);
    CollisionInfo i (hit, r);
    EXPECT_EQ(i.get_impact().get_t(), 4);
    EXPECT_EQ(i.get_impact().get_obj(), s);
    EXPECT_EQ(i.get_pnt(), Tuple({0,0,-1},TupType::POINT));
    EXPECT_EQ(i.get_eye(), Tuple({0,0,-1}));
    EXPECT_EQ(i.get_normal(), Tuple({0,0,-1}));
    EXPECT_EQ(i.is_inside(), false);
    r = Ray(Tuple({0,0,0}, TupType::POINT),Tuple({0,0,1}));
    hit = Impact(1,s);
    i  = CollisionInfo(hit, r);
    EXPECT_EQ(i.get_impact().get_t(), 1);
    EXPECT_EQ(i.get_impact().get_obj(), s);
    EXPECT_EQ(i.get_pnt(), Tuple({0,0,1},TupType::POINT));
    EXPECT_EQ(i.get_eye(), Tuple({0,0,-1}));
    EXPECT_EQ(i.get_normal(), Tuple({0,0,-1}));
    EXPECT_EQ(i.is_inside(), true);
 }

 TEST(CollisionInfoTest, Over){
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::shared_ptr<Sphere> s = std::make_shared<Sphere>(Sphere());
    Impact hit(4,s);
    CollisionInfo i (hit, r);
    EXPECT_LE(i.get_over_pnt()[2], -glob_resolution/2.0);
    EXPECT_GE(i.get_pnt()[2],i.get_over_pnt()[2]);
}

TEST(CollisionInfoTest, Reflect){
    Plane p;
    Ray r(Tuple({0,1,-1}, TupType::POINT), Tuple({0,-std::sqrt(2)/2.0,std::sqrt(2)/2.0}));
    Impact i(std::sqrt(2), std::make_shared<Plane>(p));
    CollisionInfo c(i,r);
    EXPECT_EQ(c.get_reflect(),Tuple({0,std::sqrt(2)/2.0,std::sqrt(2)/2.0}));
}
