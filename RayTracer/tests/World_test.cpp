#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "World.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tuple.h"

TEST(WorldTest,IntersectionTest){
    World w= default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::vector<Intersection> hits = w.intersect(r);
    EXPECT_EQ(hits.size(), 4);
    EXPECT_EQ(hits[0].get_t(),4);
    EXPECT_EQ(hits[1].get_t(),4.5);
    EXPECT_EQ(hits[2].get_t(),5.5);
    EXPECT_EQ(hits[3].get_t(),6);
}

TEST(WorldTest, IntersectionInfo){
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

 TEST(WorldTest, Lighting){
    World w = default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT), Tuple({0,0,1}));
    Intersection i(4, w.get_shape(0),r);
    std::cout << w << std::endl;
    std::cout << r << std::endl;
    std::cout << i << std::endl;
    Color c = w.shade_hit(i);
    EXPECT_EQ(c,Color({0.38066, 0.47583, 0.2855}));
 }