#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "AABB.h"
#include "BVH.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Group.h"
#include "CSG.h"
#include "Constants.h"
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

TEST(BVHTests, AddBox){
    AABB box1({-5, -2, 0}, {7, 4, 4});
    AABB box2({8, -7, -2},{14, 2, 8});
    box1.expand_box(&box2);
    EXPECT_EQ(box1.get_min(), GenPoint(-5,-7,-2));
    EXPECT_EQ(box1.get_max(), GenPoint(14, 4, 8));
}

TEST(BVHTests, Contains){
    AABB box({5,-2,0},{11,4,7});
    Tuple point = GenPoint(5,-2,0);
    EXPECT_EQ(box.contains(point), true);
    point = GenPoint(11,4,7);
    EXPECT_EQ(box.contains(point),true);
    point = GenPoint(8,1,3);
    EXPECT_EQ(box.contains(point),true);
    point = GenPoint(3, 0, 3);
    EXPECT_EQ(box.contains(point),false);
    point = GenPoint(8, -4, 3);
    EXPECT_EQ(box.contains(point),false);
    point = GenPoint(8, 1, -1);
    EXPECT_EQ(box.contains(point),false);
    point = GenPoint(13, 1, 3);
    EXPECT_EQ(box.contains(point),false);
    point = GenPoint(8, 5, 3);
    EXPECT_EQ(box.contains(point),false);
    point = GenPoint(8, 1, 8);
    EXPECT_EQ(box.contains(point),false);
}

TEST(BVHTests, ContainsBox){
    AABB main_box({5, -2, 0},{11, 4, 7});
    AABB test({5, -2, 0},{11, 4, 7});
    EXPECT_EQ(main_box.contains(test), true);
    test = AABB({6, -1, 1},{10, 3, 6});
    EXPECT_EQ(main_box.contains(test),true);
    test = AABB({4, -3, -1},{10, 3, 6});
    EXPECT_EQ(main_box.contains(test),false);
    test = AABB({6, -1, 1},{12, 5, 8});
    EXPECT_EQ(main_box.contains(test),false);
}

TEST(BVHTests, Transform){
    AABB box({-1,-1,-1},{1,1,1});
    Matrix matrix = MatRotateX(pi/4.0)*MatRotateY(pi/4.0);
    auto data = box.transform(matrix);    
    EXPECT_EQ(data->get_min(), GenPoint(-1.41421, -1.70711, -1.70711));
    EXPECT_EQ(data->get_max(), GenPoint(1.41421, 1.70711, 1.70711));
}

TEST(BVHTests, Parent){
    Sphere s(MatTranslation(1,-3,5)* MatScaling(0.5,2,4));
    auto box = parent_space_bounds(&s);
    EXPECT_EQ(box->get_min(), GenPoint(0.5,-5,1));
    EXPECT_EQ(box->get_max(), GenPoint(1.5, -1, 9));
}

TEST(BVHTests, Groups){
    Sphere s(MatTranslation(2,5,-3)* MatScaling(2,2,2));
    Cylinder cyl(MatTranslation(-4,-1,4)*MatScaling(0.5,1,0.5));
    cyl.set_min(-2);
    cyl.set_max(2);
    Group g;
    g.add_child(std::make_unique<Sphere>(std::move(s)));
    g.add_child(std::make_unique<Cylinder>(std::move(cyl)));
    auto bb = g.bound();
    EXPECT_EQ(bb->get_min(), GenPoint(-4.5,-3,-5));
    EXPECT_EQ(bb->get_max(), GenPoint(4, 7, 4.5));
}

TEST(BVHTests, CSGAgg){
    Sphere left;
    Sphere right(MatTranslation(2,3,4));
    CSG agg(CSG_OPS::DIFFERENCE, std::make_unique<Sphere>(std::move(left)), std::make_unique<Sphere>(std::move(right)));
    auto bb  = agg.bound();
    EXPECT_EQ(bb->get_min(), GenPoint(-1,-1,-1));
    EXPECT_EQ(bb->get_max(), GenPoint(3,4,5));
}

TEST(BVHTests,Split){
    AABB box({-1,-2,-3},{1,2,3});
    AABB expected_left({-1,-2,-3},{1,2,0});
    AABB expected_right({-1,-2,0},{1,2,3});
    box.split();
    EXPECT_EQ(box.get_left()->get_min(), expected_left.get_min());
    EXPECT_EQ(box.get_left()->get_max(), expected_left.get_max());
    EXPECT_EQ(box.get_right()->get_min(), expected_right.get_min());
    EXPECT_EQ(box.get_right()->get_max(), expected_right.get_max());
}

TEST(BVHTests, Insert){
    auto small_box =  std::make_unique<AABB>(AABB({-1,-1,-1},{1,1,1}));
    auto big_box =  std::make_unique<AABB>(AABB({-2,-2,-2},{2,2,2}));
// big_box shouldn't fit in in small_box
    EXPECT_EQ(small_box->insert(big_box), false);
// big_box should not get transferred to small_box
    EXPECT_NE(big_box, nullptr);
// small_box should fit in big_box
    EXPECT_EQ(big_box->insert(small_box), true);
// small_box should be nullptr
    EXPECT_EQ(small_box, nullptr);
// big_box should have 1 in the center
    ASSERT_EQ(big_box->get_center().size(), 1);
    EXPECT_EQ(*(big_box->get_center()[0]), AABB({-1,-1,-1},{1,1,1}));
// add tiny box should also go into center
    auto tiny_box =  std::make_unique<AABB>(AABB({-0.1,1.5,1.5},{0.1,1.6,1.6}));
    EXPECT_EQ(big_box->insert(tiny_box), true);
    EXPECT_EQ(tiny_box, nullptr);
    ASSERT_EQ(big_box->get_center().size(), 2);
    EXPECT_EQ(*(big_box->get_center()[1]), AABB({-0.1,1.5,1.5},{0.1,1.6,1.6}));
// upon insert, left and right should get updated
    EXPECT_EQ(big_box->get_left(), nullptr);
    EXPECT_EQ(big_box->get_right(), nullptr);
    auto left_box = std::make_unique<AABB>(AABB({0.1,0.1,0.1},{0.5,0.5,0.5}));
    EXPECT_EQ(big_box->insert(left_box),true);
    EXPECT_NE(big_box->get_left(), nullptr);
    EXPECT_NE(big_box->get_right(), nullptr);
}

TEST(BVHTests, Print){
    auto value = Sphere().bound();
    value->print();
    auto bbox = AABB();
    bbox.print();
}

TEST(BVHTests, InsertShapes){
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatScaling(2,2,2)))));
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatTranslation(2,2,2)))));
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatTranslation(-2,-2,-2)))));
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatScaling(0.5,0.5,0.5)*MatTranslation(-2,-2,-2)))));
    auto  a = BVH(shapes);
    EXPECT_EQ(a.count_leaves(),4);
}

TEST(BVHTests, Intersection){
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatTranslation(0,0,0.5)))));
    shapes.push_back(std::move(std::make_unique<Sphere>(Sphere(MatTranslation(0,0,-5)))));
    Ray r({0,0,2},{0,0,-1});
    auto  a = BVH(shapes);
    auto hits = a.intersect(r);
    ASSERT_EQ(hits.size(),4);
    EXPECT_EQ(hits[0].get_t(), 0.5);
    EXPECT_EQ(hits[1].get_t(), 2.5);
    EXPECT_EQ(hits[2].get_t(), 6);
    EXPECT_EQ(hits[3].get_t(), 8);
}

TEST(BVHTests,Intersect){
    AABB box({5,-2,0},{11,4,7});
    Ray r({15,1,2},{-1,0,0});
    EXPECT_EQ(box.intersect(r),true);
    r = Ray({-5,-1,4},{1,0,0});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({7, 6, 5},{0, -1, 0});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({9, -5, 6},{0, 1, 0});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({8, 2, 12},{0, 0, -1});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({6, 0, -5},{0, 0, 1});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({8, 1, 3.5},{0, 0, 1});
    EXPECT_EQ(box.intersect(r), true);
    r = Ray({9, -1, -8},{2, 4, 6});
    EXPECT_EQ(box.intersect(r), false);
    r = Ray({8, 3, -4},{6, 2, 4});
    EXPECT_EQ(box.intersect(r), false);
    r = Ray({9, -1, -2},{4, 6, 2});
    EXPECT_EQ(box.intersect(r), false);
    r = Ray({4, 0, 9},{0, 0, -1});
    EXPECT_EQ(box.intersect(r), false);
    r = Ray({8, 6, -1},{0, -1, 0});
    EXPECT_EQ(box.intersect(r), false);
    r = Ray({12, 5, 4},{-1, 0, 0});
    EXPECT_EQ(box.intersect(r), false);
}

TEST(BVHTests, ReadWorld){
    auto w = default_world();
    w->init_bvh();
    w->get_bvh()->print();
    EXPECT_EQ(w->number_of_shapes(),w->get_bvh()->count_leaves() );
}