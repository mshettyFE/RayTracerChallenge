#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "AABB.h"
#include "BVH.h"

TEST(BVH_Tests,NodeInitializationAndComparison){
    AABB Node1({1,2,3},{4,5,6});
    AABB Node2({1,2,3},{4,5,6});
    AABB Node3({1,2,3},{4,5,7});
    EXPECT_EQ(Node1,Node2);
    EXPECT_NE(Node1,Node3);
}