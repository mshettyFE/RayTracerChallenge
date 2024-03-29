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
    std::unique_ptr<Canvas> img2 = cam.render(wrd.get());
    EXPECT_EQ((*img2)(5,5), Color({0.38066, 0.47583, 0.2855}));
}