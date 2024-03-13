#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Camera.h"
#include "Matrix.h"
#include "Sphere.h"
#include "Shape.h"
#include "PointSource.h"
#include "LightSource.h"
#include "World.h"

TEST(CameraTests, ViewTransformations){
// identity
    Tuple from({0,0,0},TupType::POINT);
    Tuple to({0,0,-1},TupType::POINT);
    Tuple upwards({0,1,0});
    Camera c(1,1,1,from,to,upwards);
    Matrix expected = MatIdentity(4);
    EXPECT_EQ(c.get_view(), expected);
// scaled
    from = Tuple({0,0,0}, TupType::POINT);
    to = Tuple({0,0,1}, TupType::POINT);
    upwards = Tuple({0,1,0});
    c = Camera(1,1,1,from,to,upwards);
    expected = MatScaling(-1,1,-1);
    EXPECT_EQ(c.get_view(), expected);
// Translated
    from = Tuple({0,0,8}, TupType::POINT);
    to = Tuple({0,0,0}, TupType::POINT);
    upwards = Tuple({0,1,0});
    c = Camera(1,1,1,from,to,upwards);
    expected = MatTranslation(0,0,-8);
    EXPECT_EQ(c.get_view(), expected);
// Arbitrary
    from = Tuple({1,3,2}, TupType::POINT);
    to = Tuple({4,-2,8}, TupType::POINT);
    upwards = Tuple({1,1,0});
    c = Camera(1,1,1,from,to,upwards);
    expected = Matrix({
        {-0.50709 , 0.50709 , 0.67612 , -2.36643},
        {0.76772 , 0.60609 , 0.12122 , -2.82843},
        {-0.35857 , 0.59761 , -0.71714 , 0.00000},
        {0.00000 , 0.00000 , 0.00000 , 1.00000}
    });
    EXPECT_EQ(c.get_view(), expected);
}

TEST(CameraTests, PixelSize){
    Camera  c(200, 125, pi/2.0);
    double expected = 0.01;
    EXPECT_DOUBLE_EQ(c.get_pixel_size(),expected);
    Camera  d(125, 200, pi/2.0);
    EXPECT_DOUBLE_EQ(c.get_pixel_size(),expected);
}

TEST(CameraTests, ShootRay){
    Camera c(201,101, pi/2.0);
    Ray r = c.ray_for_pixel(100,50);
    Ray expected = Ray({0,0,0}, {0,0,-1});
    EXPECT_EQ(r, expected);
    r = c.ray_for_pixel(0,0);
    expected = Ray({0,0,0}, {0.66519, 0.33259, -0.66851});
    EXPECT_EQ(r, expected);
    Matrix new_view = MatRotateY(pi/4.0)* MatTranslation(0,-2,5);
    c.set_view(new_view);
    r = c.ray_for_pixel(100,50);
    expected = Ray({0,2,-5}, {std::pow(2,0.5)/2.0,0,-std::pow(2,0.5)/2.0});
    EXPECT_EQ(r, expected);
}

TEST(CameraTests, Render){
    auto wrd = default_world();
    Tuple from = Tuple({0,0,-5}, TupType::POINT);
    Tuple to =  Tuple({0,0,0}, TupType::POINT);
    Tuple up = Tuple({0,1,0});
    Camera cam(11,11,pi/5.0, from, to, up);
    std::unique_ptr<Canvas> img2 = cam.render(*wrd);
    EXPECT_EQ((*img2)(5,5), Color({0.38066, 0.47583, 0.2855}));
}
/*

TEST(TestImage, OnlySpheres){
// floor
    Material floor_mat = Material();
    floor_mat.set_color(Color({1, 0.9, 0.9}));
    floor_mat.set_specular(0);
    Sphere floor = Sphere(MatScaling(10,0.01,10), floor_mat);
// left wall
    Material wall_mat = floor_mat;
    Matrix left_wall_transformation = MatTranslation(0,0,5)*MatRotateY(-pi/4.0)*MatRotateX(pi/2.0)*MatScaling(10,0.01,10);
    Sphere lwall = Sphere(left_wall_transformation, wall_mat);
// right wall
    Matrix right_wall_transformation = MatTranslation(0,0,5)*MatRotateY(pi/4.0)*MatRotateX(pi/2.0)*MatScaling(10,0.01,10);
    Sphere rwall = Sphere(right_wall_transformation, wall_mat);
// middle sphere
    Matrix middle_transform = MatTranslation(-0.5,1,0.5);
    Material middle_mat = Material();
    middle_mat.set_diffuse(0.7);
    middle_mat.set_specular(0.3);
    middle_mat.set_color(Color({0.1,1,0.5}));
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
    PointSource ps(WHITE, Tuple({10,10,-10}, TupType::POINT));
// camera
    Tuple from = Tuple({0,1.5,-5}, TupType::POINT);
    Tuple to = Tuple({0,1,0}, TupType::POINT);
    Tuple up = Tuple({1,0,0});
    Camera c(100,100, pi/3.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Sphere>(floor));
    w.add_shape(std::make_unique<Sphere>(lwall));
    w.add_shape(std::make_unique<Sphere>(rwall));
    w.add_shape(std::make_unique<Sphere>(middle));
    w.add_shape(std::make_unique<Sphere>(right));
    w.add_shape(std::make_unique<Sphere>(smallest));
    w.add_source(std::make_unique<PointSource>(ps));
    std::unique_ptr<Canvas> img = c.render(w);
    img->save_ppm("Scene");
}

TEST(TestImage,ShadedSphereRedux){
    // unit sphere at origin
    Sphere s(MatIdentity(4), Material(Color(1,0.2,1)));
    // Place light source up, behind and left the camera
    PointSource light_loc(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT));
    Tuple from = Tuple({0,0,-3}, TupType::POINT);
    Tuple to = Tuple({0,0,0}, TupType::POINT);
    Tuple up = Tuple({0,1,0});
    Camera c(100,100, pi/4.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Sphere>(s));
    w.add_source(std::make_unique<PointSource>(light_loc));
    std::unique_ptr<Canvas> img = c.render(w);
    img->save_ppm("ShadedSphereCamera");
}

*/