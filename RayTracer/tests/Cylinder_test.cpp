#include "PointSource.h"
#include "LightSource.h"
#include "World.h"
#include "Camera.h"
#include "Canvas.h"
#include "Cylinder.h"
#include <gtest/gtest.h>

TEST(CylinderTest, Miss){
    Cylinder c;
    Tuple direction({0,1,0});
    direction.normalize();
    Tuple origin({1,0,0}, TupType::POINT);
    Ray r(origin,direction);
    EXPECT_EQ(c.intersect(r).size() ,0);
    direction = Tuple({0,0,0});
    direction.normalize();
    origin = Tuple({0,1,0}, TupType::POINT);
    r = Ray(origin,direction);
    EXPECT_EQ(c.intersect(r).size() ,0);
    direction = Tuple({1,1,1});
    direction.normalize();
    origin = Tuple({0,0,-5}, TupType::POINT);
    r = Ray(origin,direction);
    EXPECT_EQ(c.intersect(r).size() ,0);
}


TEST(CylinderTest, Hit){
    Cylinder c;
    Tuple direction({0,0,1});
    direction.normalize();
    Tuple origin({1,0,-5}, TupType::POINT);
    Ray r(origin, direction);
    auto hits= c.intersect(r);
    ASSERT_EQ(hits.size(), 2);
    EXPECT_EQ(hits[0].get_t(),5);
    EXPECT_EQ(hits[1].get_t(),5);
    direction = Tuple({0,0,1});
    direction.normalize();
    origin = Tuple({0,0,-5}, TupType::POINT);
    r = Ray(origin, direction);
    hits= c.intersect(r);
    EXPECT_EQ(hits.size(), 2);
    EXPECT_EQ(hits[0].get_t(),4);
    EXPECT_EQ(hits[1].get_t(),6);
    direction = Tuple({0.1,1,1});
    direction.normalize();
    origin = Tuple({0.5,0,-5}, TupType::POINT);
    r = Ray(origin, direction);
    hits= c.intersect(r);
    EXPECT_EQ(hits.size(), 2);
    EXPECT_LE(hits[0].get_t()-6.80798,glob_resolution);
    EXPECT_LE(hits[1].get_t()-7.08872,glob_resolution);
}


TEST(CylinderTest, Normal){
    Cylinder c;
    Tuple pnt({1,0,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({1,0,0}));
    pnt = Tuple({0,5,-1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,0,-1}));
    pnt = Tuple({0,-2,1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,0,1}));
    pnt = Tuple({-1,1,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({-1,0,0}));
}

TEST(CylinderTest, Truncate){
    Cylinder c;
    c.set_max(2);
    c.set_min(1);
    Tuple direction({0.1,1,0});
    direction.normalize();
    Tuple origin({0,1.5,0}, TupType::POINT);
    Ray r(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 0);
    direction = Tuple({0,0,1});
    direction.normalize();
    origin = Tuple({0,3,-5}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 0);
    origin = Tuple({0,0,-5}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 0);
    origin = Tuple({0,2,-5}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 0);
    origin = Tuple({0,1,-5}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 0);
    origin = Tuple({0,1.5,-2}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
}

TEST(CylinderTest, CappedIntersection){
    Cylinder c;
    c.set_max(2);
    c.set_min(1);
    c.set_closed(true);
    Tuple direction({0,-1,0});
    direction.normalize();
    Tuple origin({0,3,0}, TupType::POINT);
    Ray r(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
    direction = Tuple({0,-1,2});
    direction.normalize();
    origin = Tuple({0,3,-2}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
    direction = Tuple({0,-1,1});
    direction.normalize();
    origin = Tuple({0,4,-2}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
    direction = Tuple({0,1,2});
    direction.normalize();
    origin = Tuple({0,0,-2}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
    direction = Tuple({0,1,1});
    direction.normalize();
    origin = Tuple({0,-1,-2}, TupType::POINT);
    r = Ray(origin, direction);
    EXPECT_EQ(c.intersect(r).size(), 2);
}

TEST(CylinderTest, CappedNormal){
    Cylinder c;
    c.set_max(2);
    c.set_min(1);
    c.set_closed(true);
    Tuple pnt({0,1,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,-1,0}));
    pnt = Tuple({0.5,1,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,-1,0}));
    pnt = Tuple({0,1,0.5}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,-1,0}));
    pnt = Tuple({0,2,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,1,0}));
    pnt = Tuple({0.5,2,0}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,1,0}));
    pnt = Tuple({0,2,0.5}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0,1,0}));
}