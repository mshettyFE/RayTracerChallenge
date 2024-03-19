#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "CSG.h"

#include "Sphere.h"
#include "Cube.h"

TEST(CSG_TEST, Creation){
    CSG temp(CSG_OPS::UNION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    EXPECT_EQ(temp.get_left()->get_parent() , temp.get_right()->get_parent());
}

TEST(CSG_TEST, Union){
    CSG temp(CSG_OPS::UNION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    EXPECT_EQ(temp.intersection_allowed(true,true,true), false);
    EXPECT_EQ(temp.intersection_allowed(true,true,false), true);
    EXPECT_EQ(temp.intersection_allowed(true,false,true), false);
    EXPECT_EQ(temp.intersection_allowed(true,false,false), true);
    EXPECT_EQ(temp.intersection_allowed(false,true,true), false);
    EXPECT_EQ(temp.intersection_allowed(false,true,false), false);
    EXPECT_EQ(temp.intersection_allowed(false,false,true), true);
    EXPECT_EQ(temp.intersection_allowed(false,false,false), true);
}

TEST(CSG_TEST, Intersect){
    CSG temp(CSG_OPS::INTERSECTION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    EXPECT_EQ(temp.intersection_allowed(true,true,true), true);
    EXPECT_EQ(temp.intersection_allowed(true,true,false), false);
    EXPECT_EQ(temp.intersection_allowed(true,false,true), true);
    EXPECT_EQ(temp.intersection_allowed(true,false,false), false);
    EXPECT_EQ(temp.intersection_allowed(false,true,true), true);
    EXPECT_EQ(temp.intersection_allowed(false,true,false), true);
    EXPECT_EQ(temp.intersection_allowed(false,false,true), false);
    EXPECT_EQ(temp.intersection_allowed(false,false,false), false);
}

TEST(CSG_TEST, Difference){
    CSG temp(CSG_OPS::DIFFERENCE,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    EXPECT_EQ(temp.intersection_allowed(true,true,true), false);
    EXPECT_EQ(temp.intersection_allowed(true,true,false), true);
    EXPECT_EQ(temp.intersection_allowed(true,false,true), false);
    EXPECT_EQ(temp.intersection_allowed(true,false,false), true);
    EXPECT_EQ(temp.intersection_allowed(false,true,true), true);
    EXPECT_EQ(temp.intersection_allowed(false,true,false), true);
    EXPECT_EQ(temp.intersection_allowed(false,false,true), false);
    EXPECT_EQ(temp.intersection_allowed(false,false,false), false);
}

