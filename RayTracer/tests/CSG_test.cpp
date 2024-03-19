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

TEST(CSG_TEST, Filter){
    CSG temp(CSG_OPS::UNION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    const Shape* left = temp.get_left();
    const Shape* right = temp.get_right();
    std::vector<Impact> hits = {Impact(1,left), Impact(2,right), Impact(3,left), Impact(4,right)};
    auto filtered = temp.filter_impacts(hits);
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0], hits[0]);
    EXPECT_EQ(filtered[1], hits[3]);

    temp = CSG(CSG_OPS::INTERSECTION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    left = temp.get_left();
    right = temp.get_right();
    hits = {Impact(1,left), Impact(2,right), Impact(3,left), Impact(4,right)};
    filtered = temp.filter_impacts(hits);
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0], hits[1]);
    EXPECT_EQ(filtered[1], hits[2]);

    temp = CSG(CSG_OPS::DIFFERENCE,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    left = temp.get_left();
    right = temp.get_right();
    hits = {Impact(1,left), Impact(2,right), Impact(3,left), Impact(4,right)};
    filtered = temp.filter_impacts(hits);
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0], hits[0]);
    EXPECT_EQ(filtered[1], hits[1]);
}

TEST(CSG_TEST, Miss){
    CSG temp(CSG_OPS::UNION,std::make_unique<Sphere>(Sphere()), std::make_unique<Cube>(Cube()));
    Ray r({0,2,-5},{0,0,1});
    auto hits = temp.local_intersect(r);
    EXPECT_EQ(hits.size(), 0);
}

TEST(CSG_TEST, Hit){
    Sphere s1;
    Sphere s2(MatTranslation(0,0,0.5));
    CSG temp(CSG_OPS::UNION,std::make_unique<Sphere>(std::move(s1)), std::make_unique<Sphere>(std::move(s2)));
    auto left = temp.get_left();
    auto right = temp.get_right();
    Ray r({0,0,-5},{0,0,1});
    auto hits = temp.local_intersect(r);
    ASSERT_EQ(hits.size(), 2);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[0].get_obj(), left);
    EXPECT_EQ(hits[1].get_t(), 6.5);
    EXPECT_EQ(hits[1].get_obj(), right);
}