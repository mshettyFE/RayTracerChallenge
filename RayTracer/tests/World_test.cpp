#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "World.h"
#include "Intersection.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tuple.h"
#include "PointSource.h"
#include <memory>

TEST(WorldTest,IntersectionTest){
    World w= default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::vector<Intersection> hits = w.intersect(r);
    EXPECT_EQ(hits.size(), 4);
    EXPECT_EQ(hits[0].get_t(),4);
    EXPECT_EQ(hits[1].get_t(),4.5);
    EXPECT_EQ(hits[2].get_t(),5.5);
    EXPECT_EQ(hits[3].get_t(),6);
}

 TEST(WorldTest, Lighting){
    World w = default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT), Tuple({0,0,1}));
    Intersection i(4, w.get_shape(0),r);
    std::cout << "World" << std::endl;
    std::cout << w << std::endl;
    std::cout << i << std::endl;
    std::cout << r << std::endl;
    Color c = w.shade_hit(i);
    EXPECT_EQ(c,Color({0.38066, 0.47583, 0.2855}));
 }

 TEST(WorldTest, InsideLight){
    std::shared_ptr<PointSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({0,0.25,0}, TupType::POINT)));
    std::vector<std::shared_ptr<PointSource>> sources;
    sources.push_back(source);
    Material mat1(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    Material mat2(0.1,0.9,0.9,200.0, WHITE);
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Sphere(MatIdentity(4),mat1));
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Sphere(MatScaling(0.5,0.5,0.5),mat2));
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(s1);
    shapes.push_back(s2);
    World w = World(sources, shapes);
    Ray r(Tuple({0,0,0}, TupType::POINT), Tuple({0,0,1}));
    Intersection i(0.5, w.get_shape(1),r);
    std::cout << "World" << std::endl;
    std::cout << w << std::endl;
    std::cout << i << std::endl;
    std::cout << r << std::endl;
    Color c = w.shade_hit(i);
    EXPECT_EQ(c,Color({0.90498, 0.90498, 0.90498}));
 }
 
 TEST(WorldTest, Miss){
    World w =  default_world();
    Ray r  = Ray(Tuple({0, 0, -5}, TupType::POINT), Tuple({0, 1, 0}));
    Color c = w.color_at(r);
    EXPECT_EQ(c, BLACK);
}

TEST(WorldTest,Hit){
    World w =  default_world();
    Ray r  = Ray(Tuple({0, 0, -5}, TupType::POINT), Tuple({0, 0, 1}));
    std::cout << "World" << std::endl;
    std::cout << w << std::endl;
    std::cout << r << std::endl;
    Color c = w.color_at(r);
    EXPECT_EQ(c, Color({0.38066, 0.47583, 0.2855}));
}


TEST(WorldTest, Nested){
    Material mat(1,0.9,0.9,200.0,Color({0.8,1.0,0.6}));
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Sphere(MatIdentity(4),mat));
    Material mat2(1,0.9,0.9,200.0,Color({1,1,1}));
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Sphere(MatScaling(0.5,0.5,0.5),mat2));
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(s1);
    shapes.push_back(s2);
    std::shared_ptr<PointSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT)));
    std::vector<std::shared_ptr<PointSource>> sources;
    sources.push_back(source);
    World w = World(sources, shapes);
    Ray r =  Ray(Tuple({0, 0, 0.75}, TupType::POINT), Tuple({0, 0, -1}));
    std::cout << "World" << std::endl;
    std::cout << w << std::endl;
    std::cout << r << std::endl;
    Color c = w.color_at(r);
    EXPECT_EQ(c, w.get_shape(1)->get_material().get_color());
}