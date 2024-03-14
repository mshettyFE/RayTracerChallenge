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
