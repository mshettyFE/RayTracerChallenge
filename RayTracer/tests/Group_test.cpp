#include "PointSource.h"
#include "LightSource.h"
#include "World.h"
#include "Camera.h"
#include "Canvas.h"
#include "Group.h"
#include "Sphere.h"
#include <gtest/gtest.h>

TEST(GroupTest,NewChild){
    Group g;
    EXPECT_EQ(g.get_transform(),MatIdentity(4));
    EXPECT_EQ(g.get_total_children(), 0);
    Sphere s;
    g.add_child<Sphere>(s);
    EXPECT_EQ(s.get_parent(), g.get_group_pointer());
}

TEST(GroupTest, Printing){
    Group g1;
    Sphere s1;
    g1.add_child<Sphere>(s1);
    Sphere s2;
    g1.add_child<Sphere>(s2);
    Group g2;
    Sphere s3;
    g2.add_child<Sphere>(s3);
    Sphere s4;
    g2.add_child<Sphere>(s4);
    g1.add_child<Group>(g2);
    g1.verbose_print();
}

TEST(GroupTest,Intersection){
    Group g1;
    Sphere s1;
    Sphere s2(MatTranslation(0,0,-3));
    Sphere s3(MatTranslation(5,0,0));
    g1.add_child<Sphere>(s1);
    g1.add_child<Sphere>(s2);
    g1.add_child<Sphere>(s3);
    Ray r({0,0,-5},{0,0,1});
    auto hits = g1.intersect(r);
    EXPECT_EQ(hits.size(),4);
}

TEST(GroupTest, Transformation){
    Group g(MatScaling(2,2,2));
    Sphere s(MatTranslation(5,0,0));
    g.add_child<Sphere>(s);
    Ray r({10,0,-10},{0,0,1});
    auto hits = g.intersect(r);
    EXPECT_EQ(hits.size(), 2);
}

TEST(GroupTest, WorldToObj){
    Group g1(MatRotateY(pi/2.0));
    Group g2(MatScaling(2,2,2));
    g1.add_child<Group>(g2);
    Sphere s(MatTranslation(5,0,0));
    g2.add_child<Sphere>(s);
    
}