#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Sphere.h"

TEST(SphereTest, Intersection){
    Ray r(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    Matrix Trans = MatScaling(2,2,2);
    Sphere s(Trans);
    std::vector<Impact> intersections = s.intersect(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].get_t(),3.0);
    EXPECT_EQ(intersections[1].get_t(),7.0);
    r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    Trans = MatTranslation(5,0,0);
    s = Sphere(Trans);
    intersections = s.intersect(r);
    EXPECT_EQ(intersections.size(), 0);
}

TEST(SphereTest, Normals){
    // Simple Translation
    Sphere s(MatTranslation(0,1,0));
    Tuple cand({0,1.70711,-0.70711}, TupType::POINT);
    Tuple n = s.normal_at(cand);
    Tuple true_norm({0,.70711, -0.70711});
    EXPECT_EQ(n,true_norm);
    // Combination
    Matrix combo = MatRotateZ(acos(-1)/5.0)*MatScaling(1,0.5,1);
    s = Sphere(combo);
    cand = Tuple({0,std::sqrt(2)/2.0,-std::sqrt(2)/2.0}, TupType::POINT);
    true_norm = Tuple({0,0.97014,-.24254});
}