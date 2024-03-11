#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Stripes.h"
#include "Canvas.h"
#include "Material.h"
#include "PointSource.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "World.h"
#include "PointSource.h"

TEST(StripeTest,ProperSetup){
    Stripes s(WHITE, BLACK);
    EXPECT_EQ(s.at(Tuple({0,0,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,1,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,2,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,0,1},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0,0,2},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({0.9,0,0},TupType::POINT)), WHITE);
    EXPECT_EQ(s.at(Tuple({1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-0.1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-1,0,0},TupType::POINT)), BLACK);
    EXPECT_EQ(s.at(Tuple({-1.1,0,0},TupType::POINT)), WHITE);
}

TEST(StripeTest, Lighting){
    Stripes s(WHITE, BLACK);
    Material mat;
    mat.set_ambient(1);
    mat.set_diffuse(0);
    mat.set_specular(0);
    mat.set_pattern(std::make_shared<Stripes>(s));
    Tuple eye({0,0,-1});
    Tuple normal({0,0,-1});
    PointSource ps(WHITE, Tuple({0,0,-10}, TupType::POINT));
    Tuple point({0.9,0,0}, TupType::POINT);
    Sphere sph(MatIdentity(4), mat);
    EXPECT_EQ(ps.shade(std::make_shared<Sphere>(sph),point,eye,normal,false), WHITE);
    point = Tuple({1.1,0,0}, TupType::POINT);
    EXPECT_EQ(ps.shade(std::make_shared<Sphere>(sph),point,eye,normal,false), BLACK);
}

TEST(StripeTest, Transform1){
    Material mat;
    auto pat = Stripes(WHITE,BLACK);
    mat.set_pattern(std::make_shared<Stripes>(pat));
    Sphere s(MatScaling(2,2,2),mat);
    EXPECT_EQ(pat.at_object(Tuple({1.5,0,0}, TupType::POINT), std::make_shared<Sphere>(s)),WHITE);
}

TEST(StripeTest, Transform2){
    Material mat;
    auto pat = Stripes(WHITE,BLACK,MatScaling(2,2,2));
    mat.set_pattern(std::make_shared<Stripes>(pat));
    Sphere s(MatIdentity(4),mat);
    EXPECT_EQ(pat.at_object(Tuple({1.5,0,0}, TupType::POINT), std::make_shared<Sphere>(s)),WHITE);
}

TEST(StripeTest, Transform3){
    Material mat;
    auto pat = Stripes(WHITE,BLACK, MatTranslation(0.5,0,0));
    mat.set_pattern(std::make_shared<Stripes>(pat));
    Sphere s(MatScaling(2,2,2),mat);
    EXPECT_EQ(pat.at_object(Tuple({2.5,0,0}, TupType::POINT), std::make_shared<Sphere>(s)),WHITE);
}

TEST(TestImage, PatternTests){
// floor
    Material floor_mat = Material();
    floor_mat.set_color(Color({1, 0.9, 0.9}));
    floor_mat.set_specular(0);
    Plane floor = Plane(MatScaling(10,0.01,10), floor_mat);
// middle sphere
    Matrix middle_transform = MatTranslation(-0.5,1,0.5);
    Material middle_mat = Material();
    middle_mat.set_diffuse(0.7);
    middle_mat.set_specular(0.3);
    middle_mat.set_color(Color({0.1,1,0.5}));
    middle_mat.set_pattern(std::make_shared<Stripes>(Stripes(WHITE,BLACK, MatScaling(0.5,0.5,0.5))));
    middle_mat.set_cast_shadow(false);
    Sphere middle = Sphere(middle_transform, middle_mat);
// right sphere
    Matrix right_transform = MatTranslation(1.5,0.5,-0.5)*MatScaling(0.5,0.5,0.5);
    Material right_mat = middle_mat;
    right_mat.set_color(Color({0.5,1,0.1}));
    right_mat.set_diffuse(0.7);
    right_mat.set_specular(0.3);
    Sphere right = Sphere(right_transform, right_mat);
// smallest sphere
    Matrix smallest_trans = MatTranslation(-1.5,0.33,-0.75)*MatScaling(0.33,0.33,0.33);
    Material smallest_mat = Material();
    smallest_mat.set_diffuse(0.7);
    smallest_mat.set_specular(0.3);
    smallest_mat.set_color(Color({0.1,0.8,0.1}));
    Sphere smallest = Sphere(smallest_trans, smallest_mat);
// Light source
    PointSource ps(WHITE, Tuple({-10,10,-10}, TupType::POINT));
// camera
    Tuple from = Tuple({0,1.5,-5}, TupType::POINT);
    Tuple to = Tuple({0,1,0}, TupType::POINT);
    Tuple up = Tuple({1,0,0});
    Camera c(100,100, pi/3.0, from, to, up );
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Plane>(floor));
    shapes.push_back(std::make_shared<Sphere>(middle));
    shapes.push_back(std::make_shared<Sphere>(right));
    shapes.push_back(std::make_shared<Sphere>(smallest));
    std::vector<std::shared_ptr<LightSource>> sources;
    sources.push_back(std::make_shared<PointSource>(ps));
    World w(sources,shapes);
    std::unique_ptr<Canvas> img = c.render(w);
    img->save_ppm("StripePlaneScene");
}