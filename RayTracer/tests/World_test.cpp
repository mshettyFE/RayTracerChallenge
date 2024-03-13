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
#include "Plane.h"
#include "PointSource.h"
#include "Camera.h"
#include "Canvas.h"
#include "TestPattern.h"
#include "Checkers.h"
#include "Stripes.h"
#include "GradientPattern.h"
#include "RingPattern.h"
#include <memory>

TEST(WorldTest,IntersectionTest){
    auto w= default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    std::vector<Impact> hits = w->intersect(r);
    EXPECT_EQ(hits.size(), 4);
    EXPECT_EQ(hits[0].get_t(),4);
    EXPECT_EQ(hits[1].get_t(),4.5);
    EXPECT_EQ(hits[2].get_t(),5.5);
    EXPECT_EQ(hits[3].get_t(),6);
}

 TEST(WorldTest, Lighting){
    auto w = default_world();
    Ray r = Ray(Tuple({0,0,-5}, TupType::POINT), Tuple({0,0,1}));
    Impact hit(4,w->get_shape(0));
    CollisionInfo i(hit,r);
    Color c = w->shade_hit(i);
    EXPECT_EQ(c,Color({0.38066, 0.47583, 0.2855}));
 }

 TEST(WorldTest, InsideLight){
    std::unique_ptr<LightSource> source = std::make_unique<PointSource>(PointSource(Color(1,1,1), Tuple({0,0.25,0}, TupType::POINT)));
    Material mat1(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    Material mat2(0.1,0.9,0.9,200.0, WHITE);
    mat1.set_reflectance(0);
    mat1.set_transparency(0);
    mat2.set_reflectance(0);
    mat2.set_transparency(0);
    std::unique_ptr<Sphere> s1 = std::make_unique<Sphere>(Sphere(MatIdentity(4),mat1));
    std::unique_ptr<Sphere> s2 = std::make_unique<Sphere>(Sphere(MatScaling(0.5,0.5,0.5),mat2));
    World w;
    w.add_shape(std::move(s1));
    w.add_shape(std::move(s2));
    w.add_source(std::move(source));
    Ray r(Tuple({0,0,0}, TupType::POINT), Tuple({0,0,1}));
    Impact hit(0.5,w.get_shape(1));
    CollisionInfo i(hit,r);
    std::cout << i << std::endl;
    Color c = w.shade_hit(i);
    EXPECT_EQ(c,Color({0.90498, 0.90498, 0.90498}));
}
 
 TEST(WorldTest, Miss){
    auto w =  default_world();
    Ray r  = Ray(Tuple({0, 0, -5}, TupType::POINT), Tuple({0, 1, 0}));
    Color c = w->color_at(r);
    EXPECT_EQ(c, BLACK);
}

TEST(WorldTest,Hit){
    auto w =  default_world();
    Ray r  = Ray(Tuple({0, 0, -5}, TupType::POINT), Tuple({0, 0, 1}));
    Color c = w->color_at(r);
    EXPECT_EQ(c, Color({0.38066, 0.47583, 0.2855}));
}


TEST(WorldTest, Nested){
    Material mat(1,0.9,0.9,200.0,Color({0.8,1.0,0.6}));
    std::unique_ptr<Sphere> s1 = std::make_unique<Sphere>(Sphere(MatIdentity(4),mat));
    Material mat2(1,0.9,0.9,200.0,Color({1,1,1}));
    std::unique_ptr<Sphere> s2 = std::make_unique<Sphere>(Sphere(MatScaling(0.5,0.5,0.5),mat2));
    std::unique_ptr<PointSource> source = std::make_unique<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT)));
    World w;
    w.add_shape(std::move(s1));
    w.add_shape(std::move(s2));
    w.add_source(std::move(source));
    Ray r =  Ray(Tuple({0, 0, 0.75}, TupType::POINT), Tuple({0, 0, -1}));
    Color c = w.color_at(r);
    EXPECT_EQ(c, w.get_shape(1)->get_material().get_color());
}

TEST(WorldTest, InShadow){
    auto w = default_world();
    Tuple p = Tuple({0,10,0}, TupType::POINT);
    EXPECT_EQ(false,w->is_shadowed(p));
    p = Tuple({10,-10,10}, TupType::POINT);
    EXPECT_EQ(true,w->is_shadowed(p));
    p = Tuple({-2,2,-2}, TupType::POINT);
    EXPECT_EQ(false,w->is_shadowed(p));
    p = Tuple({-20,20,-20}, TupType::POINT);
    EXPECT_EQ(false,w->is_shadowed(p));
}

TEST(WorldTest, Shadows){
    PointSource ps(WHITE, Tuple({0,0,-10}, TupType::POINT));
    Sphere s1;
    Sphere s2(MatTranslation(0,0,10));
    Ray r  = Ray(Tuple({0, 0, 5}, TupType::POINT), Tuple({0, 0, 1}));
    World w;
    w.add_shape(std::make_unique<Sphere> (s1));
    w.add_shape(std::make_unique<Sphere> (s2));
    w.add_source(std::make_unique<PointSource> (ps));
    Impact i(4,w.get_shape(1));
    CollisionInfo c(i,r);
    Color out = w.shade_hit(c);
    EXPECT_EQ(out, Color({0.1,0.1,0.1}));
}

TEST(WorldTest, NonReflecting){
    Material mat(1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    std::unique_ptr<Sphere> s1 = std::make_unique<Sphere>(Sphere(MatIdentity(4),mat));
    std::unique_ptr<Sphere> s2 = std::make_unique<Sphere>(Sphere(MatScaling(0.5,0.5,0.5)));
    std::unique_ptr<PointSource> source = std::make_unique<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT)));
    World w();
    w.add_shape(std::move(s1));
    w.add_shape(std::move(s2));
    w.add_source(std::move(source));
    Ray r = Ray(Tuple({0, 0, 0}, TupType::POINT), Tuple({0, 0, 1}));
    Impact i(1,w.get_shape(1));
    CollisionInfo c(i,r);
    Color out =  w.reflect_color(c);
    EXPECT_EQ(out, BLACK);
}

TEST(WorldTest, ReflectColor){
    Material floor_mat = Material();
    floor_mat.set_reflectance(0.5);
    Plane p(MatTranslation(0,-1,0),floor_mat);
    auto w = default_world();
    w->add_shape(std::make_unique<Plane>(p));
    Ray r = Ray(Tuple({0, 0, -3}, TupType::POINT), Tuple({0, -std::sqrt(2)/2.0, std::sqrt(2)/2.0}));
    Impact i = Impact(std::sqrt(2),w->get_shape(2));
    CollisionInfo c  = CollisionInfo(i,r);
    EXPECT_EQ(c.get_reflect(), Tuple({0, std::sqrt(2)/2.0, std::sqrt(2)/2.0}));
    Color out =  w->reflect_color(c);
    EXPECT_EQ(out, Color({0.19033, 0.23791, 0.142749}));
}

TEST(WorldTest, ShadeReflection){
    Material floor_mat = Material();
    floor_mat.set_reflectance(0.5);
    Plane p(MatTranslation(0,-1,0),floor_mat);
    auto w = default_world();
    w->add_shape(std::make_unique<Plane>(p));
    Ray r = Ray(Tuple({0, 0, -3}, TupType::POINT), Tuple({0, -std::sqrt(2)/2.0, std::sqrt(2)/2.0}));
    Impact i = Impact(std::sqrt(2),w->get_shape(2));
    CollisionInfo c  = CollisionInfo(i,r);
    EXPECT_EQ(w->shade_hit(c), Color({0.87676, 0.92435, 0.829175}));
}

TEST(WorldTest, InfiniteRecursion){
    Material wall;
    wall.set_reflectance(1);
    Plane lower(MatTranslation(0,-1,0), wall);
    Plane upper(MatTranslation(0,1,0), wall);
    Ray r(Tuple({0,0,0}, TupType::POINT), Tuple({0,1,0}));
    World w;
    w.add_source(std::move(std::make_unique<PointSource>(Color({1,1,1}), Tuple({0,0,0}, TupType::POINT))));
    w.add_shape(std::make_unique<Plane>(lower));
    w.add_shape(std::make_unique<Plane>(upper));

    std::cout << "Infinite Recursion" << std::endl;
    world.color_at(r); // should terminate and not Segfault
}

TEST(WorldTest, CapRecursion){
    Material floor_mat = Material();
    floor_mat.set_reflectance(0.5);
    Plane p(MatTranslation(0,-1,0),floor_mat);
    auto w = default_world();
    w->add_shape(std::make_unique<Plane>(p));
    Ray r = Ray({0, 0, -3}, {0, -std::sqrt(2)/2.0, std::sqrt(2)/2.0});
    Impact i = Impact(std::sqrt(2),w->get_shape(2));
    CollisionInfo c  = CollisionInfo(i,r);
    EXPECT_EQ(w->reflect_color(c,0), BLACK);
}

TEST(WorldTest, Opaque){
    auto w = default_world();
    auto shape = w->get_shape(0);
    Ray r({0,0,-5},{0,0,1});
    Impact one(4,shape);
    Impact two(6,shape);
    std::vector<Impact> xs{one,two};
    CollisionInfo comps(xs[0], r, xs);
    EXPECT_EQ(w->refract_color(comps), BLACK);
}

TEST(WorldTest, MaxRecursionRefracted){
    auto w = default_world();
    const Shape* shp = w->get_shape(0);
    Material mat = shp->get_material();
    mat.set_transparency(1.0);
    mat.set_refractive_index(1.5);
    auto shape = std::make_unique<Sphere>(Sphere(shp->get_transform(), mat));
    Ray r({0,0,-5},{0,0,1});
    Impact one(4,shape.get());
    Impact two(6,shape.get());
    std::vector<Impact> xs{one,two};
    CollisionInfo comps(xs[0], r, xs);
    EXPECT_EQ(w->refract_color(comps,0), BLACK);
}

TEST(WorldTest, InternalReflect){
    auto w = default_world();
    const Shape* shp = w->get_shape(0);
    Material mat = shp->get_material();
    mat.set_transparency(1.0);
    mat.set_refractive_index(1.5);
    auto shape = std::make_unique<Sphere>(Sphere(shp->get_transform(), mat));
    Ray r({0,0,std::sqrt(2)/2.0},{0,1,0});
    Impact one(-std::sqrt(2)/2.0,shape.get());
    Impact two(std::sqrt(2)/2.0,shape.get());
    std::vector<Impact> xs{one,two};
    CollisionInfo comps(xs[1], r, xs);
    EXPECT_EQ(w->refract_color(comps,5), BLACK);
}

TEST(WorldTest, ColorRefracted){
    auto w = default_world();
    const Shape* a = w->get_shape(0);
    Material mat = a->get_material();
    mat.set_ambient(1.0);
    mat.set_pattern(std::make_unique<TestPattern>(TestPattern()));
    std::unique_ptr<Shape> A = std::make_unique<Sphere>(Sphere(a->get_transform(),mat));
    A->set_material(mat);
    const Shape* b = w->get_shape(1);
    mat = b->get_material();
    mat.set_transparency(1.0);
    mat.set_refractive_index(1.5);
    std::unique_ptr<Shape> B = std::make_unique<Sphere>(Sphere(b->get_transform(),mat));
    B->set_material(mat);
    w->set_shape(0,A);
    w->set_shape(0,B);
    Ray r({0,0,0.1},{0,1,0});
    std::vector<Impact> xs{Impact(-0.9899,w->get_shape(0)), Impact(-0.4899,w->get_shape(1)), Impact(0.4899,w->get_shape(1)), Impact(0.9899,w->get_shape(0))};
    CollisionInfo comps(xs[2],r, xs);
    EXPECT_EQ(w->refract_color(comps,5), Color({0, 0.99888, 0.04721}));
}

TEST(WorldTest, ShadeWithRefraction){
    auto w = default_world();
    Material plane_mat;
    plane_mat.set_transparency(0.5);
    plane_mat.set_refractive_index(1.5);
    Plane p(MatTranslation(0,-1,0),plane_mat);
    w->add_shape(std::make_unique<Plane>(p));
    Material ball_mat;
    ball_mat.set_color(Color({1,0,0}));
    ball_mat.set_ambient(0.5);
    Sphere ball(MatTranslation(0,-3.5,-0.5), ball_mat);
    w->add_shape(std::make_unique<Sphere>(ball));
    Ray r({0,0,-3},{0,-std::sqrt(2)/2.0,std::sqrt(2)/2.0});
    Impact i(std::sqrt(2), w->get_shape(2));
    CollisionInfo comps(i,r);
    EXPECT_EQ(w->shade_hit(comps), Color({0.93642, 0.68642, 0.68642}));
}

TEST(WorldTest, ReflectiveTransparentAtOnce){
    auto w = default_world();
    Material plane_mat;
    plane_mat.set_transparency(0.5);
    plane_mat.set_reflectance(0.5);
    plane_mat.set_refractive_index(1.5);
    Plane p(MatTranslation(0,-1,0),plane_mat);
    w->add_shape(std::make_unique<Plane>(p));
    Material ball_mat;
    ball_mat.set_color(Color({1,0,0}));
    ball_mat.set_ambient(0.5);
    Sphere ball(MatTranslation(0,-3.5,-0.5), ball_mat);
    w->add_shape(std::make_unique<Sphere>(ball));
    Ray r({0,0,-3}, {0,-std::sqrt(2)/2.0,std::sqrt(2)/2.0});
    Impact i(std::sqrt(2),w->get_shape(2));
    CollisionInfo comps(i,r);
    EXPECT_EQ(w->shade_hit(comps), Color({0.93391, 0.69643, 0.69243}));
}

TEST(TestImage, FresnelEffect){
    PointSource ps(WHITE,Tuple({-100,0,0}, TupType::POINT));
    Material base_mat;
    base_mat.set_reflectance(1);
    base_mat.set_pattern(std::make_unique<Checkers>(Checkers(RED, BLUE)));
    Plane base(MatTranslation(0,-1,0), base_mat);
    Material water_mat;
    water_mat.set_reflectance(0);
    water_mat.set_transparency(0.5);
    water_mat.set_refractive_index(WATER);
    Plane water(MatIdentity(4), water_mat);
    Material background_mat;
    background_mat.set_pattern(std::make_unique<Checkers>(Checkers(WHITE, BLACK)));
    Plane back(MatTranslation(100,0,0)*MatRotateZ(pi/2.0));
    World w;
    w.add_shape(std::make_unique<Plane>(base));
    w.add_shape(std::make_unique<Plane>(water));
    w.add_shape(std::make_unique<Plane>(back));
    w.add_source(std::make_unique<PointSource>(ps));
    Tuple from({-10,1,0}, TupType::POINT);
    Tuple to({0,1,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);
    auto img = cam.render(w);
    img->save_ppm("Fresnel");
}

TEST(TestImage,AirBubble){
    PointSource ps(WHITE,Tuple({-10,10,-10}, TupType::POINT));
    Material glass;
    glass.set_color(GRAY);
    glass.set_reflectance(0);
    glass.set_transparency(0);
    glass.set_refractive_index(GLASS);
    Sphere outer(MatScaling(2,2,2), glass);
    Material air;
    air.set_color(WHITE);
    air.set_reflectance(0);
    air.set_transparency(0);
    air.set_refractive_index(AIR);
    Sphere middle(MatIdentity(4), air);
    Material wall_mat;
    wall_mat.set_color(GREEN);
    air.set_reflectance(0);
    air.set_transparency(0);
    air.set_cast_shadow(false);
    auto pat =  Checkers(WHITE,RED);
    wall_mat.set_pattern(std::make_unique<Checkers>(pat));
    
    std::vector<Matrix> operations{MatRotateZ(pi/4.0),MatTranslation(7,0,0)};
    Plane wall(Chain(operations),wall_mat);
    Sphere ref(MatTranslation(0,5,0), glass);
    World w;
    w.add_shape(std::make_unique<Sphere>(outer));
    w.add_shape(std::make_unique<Sphere>(middle));
    w.add_shape(std::make_unique<Plane>(wall));
    w.add_source(std::make_unique<PointSource>(ps))
    Tuple from({-5,0,0}, TupType::POINT);
    Tuple to({0,0,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);
    auto img = cam.render(w);
    img->save_ppm("NestedGlass");
}