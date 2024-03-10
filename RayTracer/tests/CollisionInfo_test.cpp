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

TEST(CollisionInfoTest, IndexInfo){
    Sphere A = glass_sphere(1.5);
    A.set_transform(MatScaling(2,2,2));
    Sphere B = glass_sphere(2.0);
    B.set_transform(MatTranslation(-0.25,0,0));
    Sphere C = glass_sphere(2.5);
    C.set_transform(MatTranslation(0.25,0,0));
    Ray r({-4,0,0},{1,0,0});
    std::vector<Impact> hits;
    hits.push_back(Impact(2, std::make_shared<Sphere>(A)));
    hits.push_back(Impact(2.75, std::make_shared<Sphere>(B)));
    hits.push_back(Impact(3.25, std::make_shared<Sphere>(C)));
    hits.push_back(Impact(4.75, std::make_shared<Sphere>(B)));
    hits.push_back(Impact(5.25, std::make_shared<Sphere>(C)));
    hits.push_back(Impact(6, std::make_shared<Sphere>(A)));
    std::vector<CollisionInfo> comps;
    CollisionInfo cmp(hits[5],r,hits);

    for(auto hit : hits){
        comps.push_back(CollisionInfo(hit,r, hits));
    }
    EXPECT_EQ(comps[0].get_n1(), 1.0); EXPECT_EQ(comps[0].get_n2(), 1.5);
    EXPECT_EQ(comps[1].get_n1(), 1.5); EXPECT_EQ(comps[1].get_n2(), 2.0);
    EXPECT_EQ(comps[2].get_n1(), 2.0); EXPECT_EQ(comps[2].get_n2(), 2.5);
    EXPECT_EQ(comps[3].get_n1(), 2.5); EXPECT_EQ(comps[3].get_n2(), 2.5);
    EXPECT_EQ(comps[4].get_n1(), 2.5); EXPECT_EQ(comps[4].get_n2(), 1.5);
    EXPECT_EQ(comps[5].get_n1(), 1.5); EXPECT_EQ(comps[5].get_n2(), 1.0);
}

TEST(CollisionInfoTest, UnderPnt){
    Ray r({0,0,-5},{0,0,1});
    Sphere s = glass_sphere();
    s.set_transform(MatTranslation(0,0,1));
    Impact i(5.0,std::make_shared<Sphere>(s));
    CollisionInfo comps(i,r);
    EXPECT_GE(comps.get_under_pnt()[2] , glob_resolution/2.0);
    EXPECT_GE(comps.get_under_pnt()[2], comps.get_pnt()[2]);

}