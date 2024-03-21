#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "AABB.h"
#include "BVH.h"
#include "World.h"

TEST(BVHTests,NodeInitializationAndComparison){
    AABB Node1({1,2,3},{4,5,6});
    AABB Node2({1,2,3},{4,5,6});
    AABB Node3({1,2,3},{4,5,7});
    EXPECT_EQ(Node1,Node2);
    EXPECT_NE(Node1,Node3);
}

TEST(BVHTests,DefaultNode){
    AABB Node1;
    EXPECT_FLOAT_EQ(Node1.get_min_x(), INFTY);
    EXPECT_FLOAT_EQ(Node1.get_min_y(), INFTY);
    EXPECT_FLOAT_EQ(Node1.get_min_z(), INFTY);

    EXPECT_FLOAT_EQ(Node1.get_max_x(), NEG_INFTY);
    EXPECT_FLOAT_EQ(Node1.get_max_y(), NEG_INFTY);
    EXPECT_FLOAT_EQ(Node1.get_max_z(), NEG_INFTY);
}

TEST(BVHTests,AddPoint){
    AABB Node;
    Node.add_point(Tuple({-5,2,0},TupType::POINT));
    Node.add_point(Tuple({7,0,-3},TupType::POINT));
    EXPECT_FLOAT_EQ(Node.get_min_x(), -5);
    EXPECT_FLOAT_EQ(Node.get_min_y(), 0);
    EXPECT_FLOAT_EQ(Node.get_min_z(), -3);

    EXPECT_FLOAT_EQ(Node.get_max_x(), 7);
    EXPECT_FLOAT_EQ(Node.get_max_y(), 2);
    EXPECT_FLOAT_EQ(Node.get_max_z(), 0);
}

TEST(BVHTests, Expand){
    void expand_box(const AABB* new_box);

}

TEST(BVHTests, ReadWorld){
    auto w = default_world();
    w->init_bvh();
}