#include "PointSource.h"
#include "LightSource.h"
#include "World.h"
#include "Group.h"
#include "Sphere.h"
#include <gtest/gtest.h>

TEST(GroupTest,NewChild){
    Group g;
    EXPECT_EQ(g.get_transform(),MatIdentity(4));
    EXPECT_EQ(g.get_total_children(), 0);
    std::cout << "HI" << std::endl;
    const Shape* retrieved = g.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    std::cout << retrieved << std::endl;
    EXPECT_EQ(retrieved->get_parent()->get_id(),g.get_id());
}

TEST(GroupTest, Printing){
    Group g1;
    Group g2;
    g1.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    g1.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    g2.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    g2.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    g1.add_child(std::make_unique<Group>(std::move(g2)));
    g1.print();
}

TEST(GroupTest,Intersection){
    Group g1;
    g1.add_child(std::make_unique<Sphere>(std::move(Sphere())));
    g1.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(0,0,-3)))));
    g1.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(5,0,0)))));
    Ray r({0,0,-5},{0,0,1});
    auto hits = g1.intersect(r);
    EXPECT_EQ(hits.size(),4);
}

TEST(GroupTest, Transformation){
    Group g;
    g.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(5,0,0)))));
    g.set_transform(MatScaling(2,2,2));
    Ray r({10,0,-10},{0,0,1});
    auto hits = g.intersect(r);
    EXPECT_EQ(hits.size(), 2);
}

TEST(GroupTest, Normal){
    Group g1;
    Group g2(MatScaling(1,2,3));
    Group g3(MatScaling(1,2,3));
    g3.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(5,0,0)))));
    g3.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(5,0,0)))));
    g2.add_child(std::make_unique<Group>(std::move(g3)));
    std::cout << "HI" <<std::endl;
    std::cout << g2.get_this() << std::endl;
    const Shape* s1 = g2.add_child(std::make_unique<Sphere>(std::move(Sphere(MatTranslation(5,0,0)))));
    const Shape* pg2 = g1.add_child(std::make_unique<Group>(std::move(g2)));
    g1.set_transform(MatRotateY(pi/2.0));
    EXPECT_EQ(s1->normal_at(Tuple({1.7321, 1.1547, -5.5774}, TupType::POINT)),Tuple({0.285704 ,0.428543, -0.857161}));
}