#include <gtest/gtest.h>
#include "AABB.h"

static AABB unit_box(){return AABB({-1, -1, -1}, {1, 1, 1});}

TEST(AABB, RayThroughBoxHits){
    EXPECT_TRUE(unit_box().intersect(Ray({-5, 0.5, 0}, {1, 0, 0})));
    EXPECT_TRUE(unit_box().intersect(Ray({5, 5, 5}, {-1, -1, -1})));      // through the corner region
}

TEST(AABB, RayPastBoxMisses){
    EXPECT_FALSE(unit_box().intersect(Ray({-5, 2, 0}, {1, 0, 0})));
    EXPECT_FALSE(unit_box().intersect(Ray({-5, 0, 0}, {1, 1, 0})));      // passes above the box
}

// A direction component of 0 means the ray never leaves its slab on that axis: it hits only if
// its origin is already between that axis's bounds.
TEST(AABB, AxisParallelRay){
    EXPECT_TRUE(unit_box().intersect(Ray({0.5, -5, 0}, {0, 1, 0})));
    EXPECT_FALSE(unit_box().intersect(Ray({1.5, -5, 0}, {0, 1, 0})));
}

// A ray lying exactly in the plane of a face, parallel to it, counts as touching the box.
TEST(AABB, RayInFacePlaneCountsAsHit){
    EXPECT_TRUE(unit_box().intersect(Ray({1, -5, 0}, {0, 1, 0})));
}

// Parallel ray between two unbounded slabs registers a miss
TEST(AABB, UnboundedBox){
    AABB slab({-1, -1, NEG_INFTY}, {1, 1, INFTY});
    EXPECT_TRUE(slab.intersect(Ray({0, 0, -1000}, {0, 0, 1})));
    EXPECT_FALSE(slab.intersect(Ray({5, 0, -1000}, {0, 0, 1})));
}
