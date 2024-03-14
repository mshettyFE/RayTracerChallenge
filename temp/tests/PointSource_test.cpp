#include "Constants.h"
#include "Sphere.h"
#include "PointSource.h"
#include "Stripes.h"
#include <memory>
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>

TEST(PointSourceTest,Init){
    PointSource(Color({1,1,1}),Tuple({0,0,0}));
}

TEST(PointSourceTest, InShadow){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    PointSource ps(Color({1,1,1}),Tuple({0,0,-10}));
    // Straight on
    Tuple camera({0,0,-1});
    Tuple normal({0,0,-1});
    Color result = ps.shade(&s, position, camera, normal, true);
    EXPECT_EQ(result, Color(0.1,0.1,0.1));

}

TEST(PointSourceTest, StraightOn){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    PointSource ps(Color({1,1,1}),Tuple({0,0,-10}));
    // Straight on
    Tuple camera({0,0,-1});
    Tuple normal({0,0,-1});
    Color result = ps.shade(&s, position, camera, normal);
    EXPECT_EQ(result, Color(1.9,1.9,1.9));
}

TEST(PointSourceTest, AngledCamera){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    Tuple normal({0,0,-1});
    PointSource ps(Color({1,1,1}),Tuple({0,0,-10}));
    Tuple camera = Tuple({0,std::sqrt(2)/2.0,-std::sqrt(2)/2.0});
    Tuple result = ps.shade(&s, position, camera, normal);
    EXPECT_EQ(result,Color(1,1,1));
}

TEST(PointSourceTest, AngledLight){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    Tuple normal({0,0,-1});
    Tuple camera = Tuple({0,0,-1});
    PointSource ps =  PointSource(Color({1,1,1}),Tuple({0,10,-10}));
    Color result = ps.shade(&s, position, camera, normal);
    EXPECT_EQ(result, Color(.7364,.7364,.7364));
}

TEST(PointSourceTest, Obscured){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    Tuple normal({0,0,-1});
    PointSource ps =  PointSource(Color({1,1,1}),Tuple({0,0,10}));
    Tuple camera = Tuple({0,0,-1});
    Color result = ps.shade(&s,position, camera,normal);
    EXPECT_EQ(result, Color(0.1,0.1,0.1));
}

TEST(PointSourceTest, DirectLight){
// The light! It burns (camera directly in line of sight of light reflection)
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    Tuple normal({0,0,-1});
    PointSource ps(Color({1,1,1}),Tuple({0,10,-10}));
    Tuple camera = Tuple({0,-std::sqrt(2)/2.0,-std::sqrt(2)/2.0});
    Color result = ps.shade(&s, position, camera, normal);
    EXPECT_EQ(result, Color(1.6364,1.6364,1.6364));
}

TEST(PointSourceTest,PatternedObj){
    Stripes s(WHITE,BLACK);
    Material mat(1,0,0,200,RED, std::make_shared<Stripes>(s));
    Sphere sph(MatIdentity(4),mat);
    Tuple camera({0,0,-1});
    Tuple normal({0,0,-1});
    PointSource ps(Color({1,1,1}),Tuple({0,0,-10}));
    Tuple pos({0.9,0,0},TupType::POINT);
    Color c1 = ps.shade(&sph, pos, camera, normal ,false);
    pos =  Tuple({1.1,0,0},TupType::POINT);
    Color c2 = ps.shade(&sph, pos, camera, normal ,false);
    EXPECT_EQ(c1, BLACK);
    EXPECT_EQ(c2, BLACK);
}