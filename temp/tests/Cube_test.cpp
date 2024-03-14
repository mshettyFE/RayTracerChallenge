#include "PointSource.h"
#include "LightSource.h"
#include "Cube.h"
#include <gtest/gtest.h>

TEST(CubeTest, Intersection){
    Cube c;
    Ray r({5,0.5,0},{-1,0,0});
    auto hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({-5,0.5,0},{1,0,0});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({0.5,5,0},{0,-1,0});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({0.5,-5,0},{0,1,0});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({0.5,0,5},{0,0,-1});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({0.5,0,-5},{0,0,1});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), 4);
    EXPECT_EQ(hits[1].get_t(), 6);
    r = Ray({0,0.5,0},{0,0,1});
    hits = c.intersect(r);
    EXPECT_EQ(hits[0].get_t(), -1);
    EXPECT_EQ(hits[1].get_t(), 1);
}

TEST(CubeTest,Misses){
    Cube c;
    Ray r({-2,0,0},{0.2673, 0.5345, 0.8018});
    auto hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
    r = Ray({0, -2, 0},{0.8018, 0.2673, 0.5345});
    hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
    r = Ray({0, 0, -2},{0.5345, 0.8018, 0.2673});
    hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
    r = Ray({2, 0, 2},{0, 0, -1});
    hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
    r = Ray({0, 2, 2},{0, -1, 0});
    hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
    r = Ray({2, 2, 0},{-1, 0, 0});
    hits = c.intersect(r);
    EXPECT_EQ(hits.size(), 0);
}

TEST(CubeTest, Normals){
    Cube c;
    Tuple pnt({1,0.5,-0.8}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({1,0,0}));
    pnt = Tuple({-1,-0.2,0.9}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({-1,0,0}));
    pnt = Tuple({-0.4, 1, -0.1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0, 1, 0}));
    pnt = Tuple({0.3, -1, -0.7}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0, -1, 0}));
    pnt = Tuple({-0.6, 0.3, 1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0, 0, 1}));
    pnt = Tuple({0.4, 0.4, -1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({0, 0, -1}));
    pnt = Tuple({1, 1, 1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({1, 0, 0}));
    pnt = Tuple({-1, -1, -1}, TupType::POINT);
    EXPECT_EQ(c.normal_at(pnt), Tuple({-1, 0, 0}));
}