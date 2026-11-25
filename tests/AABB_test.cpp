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

// --- Median-split BVH builder ---
#include <map>
#include <memory>
#include <vector>
#include "Sphere.h"

// Walks a built tree, checking each node's box contains its children and that leaves are small,
// and counts how often each shape appears. Returns the tree depth.
static int check_tree(const AABB* node, std::map<const Shape*, int>& seen){
    if(node->get_shape()){seen[node->get_shape()]++;}
    auto leaf_objects = node->get_center();
    EXPECT_LE(leaf_objects.size(), AABB::MAX_LEAF_OBJECTS);
    if(!leaf_objects.empty()){
      EXPECT_EQ(node->get_left(), nullptr);
      EXPECT_EQ(node->get_right(), nullptr);
    }   // a node is a leaf or a split, not both
    int depth = 0;
    std::vector<const AABB*> children = leaf_objects;
    if(node->get_left()){children.push_back(node->get_left());}
    if(node->get_right()){children.push_back(node->get_right());}
    for(const AABB* child : children){
        EXPECT_TRUE(node->contains(*child));
        depth = std::max(depth, 1 + check_tree(child, seen));
    }
    return depth;
}

static std::vector<std::unique_ptr<AABB>> boxes_for(const std::vector<std::unique_ptr<Sphere>>& spheres){
    std::vector<std::unique_ptr<AABB>> boxes;
    for(auto& s : spheres){
        AABB b = s->bound();
        b.transform(s->get_transform());
        boxes.push_back(std::make_unique<AABB>(b.get_min(), b.get_max(), s.get()));
    }
    return boxes;
}

TEST(AABB, BuildBvhPlacesEveryObjectExactlyOnce){
    std::vector<std::unique_ptr<Sphere>> spheres;
    for(int i = 0; i < 1000; ++i){
        spheres.push_back(std::make_unique<Sphere>(MatTranslation((i * 37) % 101, (i * 53) % 97, (i * 71) % 89) * MatScaling(0.3, 0.3, 0.3)));
    }
    auto root = AABB::build_bvh(boxes_for(spheres));
    std::map<const Shape*, int> seen;
    int depth = check_tree(root.get(), seen);
    EXPECT_EQ(seen.size(), spheres.size());
    for(auto& [shape, count] : seen){EXPECT_EQ(count, 1);}
    EXPECT_LE(depth, 12);   // halving each level: about log2(1000 / 2) + 1, not 1000 deep
}

// Many objects with identical centres: a midpoint split would put them all on one side forever.
// The median split must still terminate, with every object placed once.
TEST(AABB, BuildBvhHandlesCoincidentCentres){
    std::vector<std::unique_ptr<Sphere>> spheres;
    for(int i = 0; i < 500; ++i){spheres.push_back(std::make_unique<Sphere>());}
    auto root = AABB::build_bvh(boxes_for(spheres));
    std::map<const Shape*, int> seen;
    int depth = check_tree(root.get(), seen);
    EXPECT_EQ(seen.size(), spheres.size());
    EXPECT_LE(depth, 10);
}

TEST(AABB, BuildBvhSmallInputs){
    EXPECT_EQ(AABB::build_bvh({}), nullptr);
    std::vector<std::unique_ptr<Sphere>> one;
    one.push_back(std::make_unique<Sphere>());
    auto root = AABB::build_bvh(boxes_for(one));
    EXPECT_EQ(root->get_shape(), one[0].get());   // a single object is its own root
}
