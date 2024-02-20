#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "World.h"
#include "Tuple.h"

TEST(WorldTest,Intersection){
    World w= default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::vector<Intersection> hits = w.intersect(r);
    EXPECT_EQ(hits.size(), 4);
    EXPECT_EQ(hits[0].t,4);
    EXPECT_EQ(hits[1].t,4.5);
    EXPECT_EQ(hits[2].t,5.5);
    EXPECT_EQ(hits[3].t,6);
}