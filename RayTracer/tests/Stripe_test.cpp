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

TEST(StripeTest, ObjTransform){
    Sphere s;
    s.set_transform(MatScaling(2,2,2));
    Stripes pat(WHITE,BLACK);
    Color c = pat.at_object(Tuple({1.5,0,0},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,WHITE);
    pat.set_transformation(MatScaling(2,2,2));
    c = pat.at_object(Tuple({1.5,0,0},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,WHITE);
    s.set_transform(MatScaling(2,2,2));
    pat.set_transformation(MatTranslation(0.5,0,0));
    c = pat.at_object(Tuple({2.5,0,0},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,WHITE);
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
    Camera c(1000,1000, pi/3.0, from, to, up );
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