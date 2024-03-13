#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "GradientPattern.h"
#include "Canvas.h"
#include "Material.h"
#include "PointSource.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "World.h"

TEST(GradPattern,Smooth){
    GradientPattern gp(WHITE, BLACK);
    Tuple pt({0,0,0}, TupType::POINT);
    EXPECT_EQ(gp.at(pt), WHITE);
    pt = Tuple({0.25,0,0}, TupType::POINT);
    EXPECT_EQ(gp.at(pt), Color({0.75,0.75,0.75}));
    pt = Tuple({0.5,0,0}, TupType::POINT);
    EXPECT_EQ(gp.at(pt), Color({0.5,0.5,0.5}));
    pt = Tuple({0.75,0,0}, TupType::POINT);
    EXPECT_EQ(gp.at(pt), Color({0.25,0.25,0.25}));
}

TEST(TestImage, GradPatternTest){
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
    middle_mat.set_pattern(std::make_unique<GradientPattern>(GradientPattern(WHITE,BLACK, MatScaling(2,2,2))));
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
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Plane>(floor));
    shapes.push_back(std::make_unique<Sphere>(middle));
    shapes.push_back(std::make_unique<Sphere>(right));
    shapes.push_back(std::make_unique<Sphere>(smallest));
    std::vector<std::unique_ptr<LightSource>> sources;
    sources.push_back(std::make_unique<PointSource>(ps));
    World w(sources, shapes);
    std::unique_ptr<Canvas> img = c.render(w);
    img->save_ppm("GradPlaneScene");
}