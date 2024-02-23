#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Intersection.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tuple.h"

TEST(IntersectionTest, IntersectionInfo){
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::shared_ptr<Sphere> s = std::make_shared<Sphere>(Sphere());
    Intersection i (4, s, r);
    EXPECT_EQ(i.get_t(), 4);
    EXPECT_EQ(i.get_obj(), s);
    EXPECT_EQ(i.get_pnt(), Tuple({0,0,-1},TupType::POINT));
    EXPECT_EQ(i.get_eye(), Tuple({0,0,-1}));
    EXPECT_EQ(i.get_normal(), Tuple({0,0,-1}));
    EXPECT_EQ(i.is_inside(), false);
    r = Ray(Tuple({0,0,0}, TupType::POINT),Tuple({0,0,1}));
    i  = Intersection(1, s, r);
    EXPECT_EQ(i.get_t(), 1);
    EXPECT_EQ(i.get_obj(), s);
    EXPECT_EQ(i.get_pnt(), Tuple({0,0,1},TupType::POINT));
    EXPECT_EQ(i.get_eye(), Tuple({0,0,-1}));
    EXPECT_EQ(i.get_normal(), Tuple({0,0,-1}));
    EXPECT_EQ(i.is_inside(), true);
 }