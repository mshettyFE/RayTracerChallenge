#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "TestPattern.h"
#include "Canvas.h"
#include "Material.h"
#include "PointSource.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "World.h"

TEST(PatternTest, ObjTransform1){
    Sphere s;
    s.set_transform(MatScaling(2,2,2));
    TestPattern pat;
    Color c = pat.at_object(Tuple({2,3,4},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,Color({1,1.5,2}));
}

TEST(PatternTest, ObjTransform2){
    Sphere s;
    TestPattern pat(MatScaling(2,2,2));
    Color c = pat.at_object(Tuple({2,3,4},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,Color({1,1.5,2}));
}

TEST(PatternTest, ObjTransform3){
    Sphere s;
    s.set_transform(MatScaling(2,2,2));
    TestPattern pat(MatTranslation(0.5, 1, 1.5));
    Color c = pat.at_object(Tuple({2.5, 3, 3.5},TupType::POINT), std::make_shared<Sphere>(s));
    EXPECT_EQ(c,Color({0.75, 0.5, 0.25}));
}

