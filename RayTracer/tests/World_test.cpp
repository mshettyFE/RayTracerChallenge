#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "World.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tuple.h"
#include "PointSource.h"
#include <memory>

TEST(WorldTest,IntersectionTest){
    World w= default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::vector<Impact> hits = w.intersect(r);
    EXPECT_EQ(hits.size(), 4);
    EXPECT_EQ(hits[0].get_t(),4);
    EXPECT_EQ(hits[1].get_t(),4.5);
    EXPECT_EQ(hits[2].get_t(),5.5);
    EXPECT_EQ(hits[3].get_t(),6);
}

 TEST(WorldTest, Lighting){
    World w = default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT), Tuple({0,0,1}));
    Impact hit(4,w.get_shape(0));
    CollisionInfo i(hit,r);
    Color c = w.shade_hit(i);
    EXPECT_EQ(c,Color({0.38066, 0.47583, 0.2855}));
 }

 TEST(WorldTest, InsideLight){
    std::shared_ptr<LightSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({0,0.25,0}, TupType::POINT)));
    std::vector<std::shared_ptr<LightSource>> sources;
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
    Impact hit(0.5,w.get_shape(1));
    CollisionInfo i(hit,r);
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
    std::vector<std::shared_ptr<LightSource>> sources;
    sources.push_back(source);
    World w = World(sources, shapes);
    Ray r =  Ray(Tuple({0, 0, 0.75}, TupType::POINT), Tuple({0, 0, -1}));
    Color c = w.color_at(r);
    EXPECT_EQ(c, w.get_shape(1)->get_material().get_color());
}

TEST(WorldTest, InShadow){
    World w = default_world();
    Tuple p = Tuple({0,10,0}, TupType::POINT);
    EXPECT_EQ(false,w.is_shadowed(p));
    p = Tuple({10,-10,10}, TupType::POINT);
    EXPECT_EQ(true,w.is_shadowed(p));
    p = Tuple({-2,2,-2}, TupType::POINT);
    EXPECT_EQ(false,w.is_shadowed(p));
    p = Tuple({-20,20,-20}, TupType::POINT);
    EXPECT_EQ(false,w.is_shadowed(p));
}

TEST(WorldTest, Shadows){
    PointSource ps(WHITE, Tuple({0,0,-10}, TupType::POINT));
    std::vector<std::shared_ptr<LightSource>> sources;
    std::vector<std::shared_ptr<Shape>> shapes;
    sources.push_back(std::make_shared<PointSource> (ps));
    Sphere s1;
    Sphere s2(MatTranslation(0,0,10));
    shapes.push_back(std::make_shared<Sphere> (s1));
    shapes.push_back(std::make_shared<Sphere> (s2));
    Ray r  = Ray(Tuple({0, 0, 5}, TupType::POINT), Tuple({0, 0, 1}));
    Impact i(4,shapes[1]);
    CollisionInfo c(i,r);
    World w(sources, shapes);
    Color out = w.shade_hit(c);
    EXPECT_EQ(out, Color({0.1,0.1,0.1}));
}