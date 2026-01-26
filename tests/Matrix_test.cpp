#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Tuple.h"
#include "Matrix.h"

TEST(MatTests, Translation){
    Matrix m = MatTranslation(5,-3,2);
    Tuple p = GenPoint(-3,4,5);
    EXPECT_EQ(m*p,GenPoint(2,1,7));
}

TEST(MatTests, InverseTranslation){
    Matrix m = MatTranslation(5,-3,2);
    Matrix inv = m.Inverse();
    Tuple p = GenPoint(-3,4,5);
    EXPECT_EQ(inv*p,GenPoint(-8,7,3));
}

TEST(MatTests, TranslationIgnoresVectors){
    Matrix m = MatTranslation(5,-3,2);
    Tuple p = GenVec(-3,4,5);
    EXPECT_EQ(m*p,p);
}

TEST(MatTests, Scaling){
    Matrix m = MatScaling(2,3,4);
    auto p = GenPoint(-4,6,8);
    EXPECT_EQ(m*p,GenPoint(-8,18,32));
}

TEST(MatTests, ScalingOnVector){
    Matrix m = MatScaling(2,3,4);
    auto p = GenVec(-4,6,8);
    EXPECT_EQ(m*p,GenVec(-8,18,32));
}

TEST(MatTests, InverseScaling){
    Matrix m = MatScaling(2,3,4);
    auto p = GenVec(-4,6,8);
    EXPECT_EQ(m.Inverse()*p,GenVec(-2,2,2));
}

TEST(MatTests, Reflection){
    auto m = MatScaling(-1,1,1);
    auto p = GenPoint(2,3,4);
    EXPECT_EQ(m*p,GenPoint(-2,3,4));
}

TEST(MatTests, RotX){
    auto half = MatRotateX(pi/4.0);
    auto full = MatRotateX(pi/2.0);
    auto p = GenPoint(0,1,0);
    EXPECT_EQ(half*p, GenPoint(0,std::sqrt(2)/2.0, std::sqrt(2)/2.0));
    EXPECT_EQ(full*p, GenPoint(0,0,1));
    p = GenPoint(0,1,0);
    EXPECT_EQ(half.Inverse()*p, GenPoint(0,std::sqrt(2)/2.0, -std::sqrt(2)/2.0));
}

TEST(MatTests, RotY){
    auto half = MatRotateY(pi/4.0);
    auto full = MatRotateY(pi/2.0);
    auto p = GenPoint(0,0,1);
    EXPECT_EQ(half*p, GenPoint(std::sqrt(2)/2.0,0, std::sqrt(2)/2.0));
    EXPECT_EQ(full*p, GenPoint(1,0,0));
}

TEST(MatTests, RotZ){
    auto half = MatRotateZ(pi/4.0);
    auto full = MatRotateZ(pi/2.0);
    auto p = GenPoint(0,1,0);
    EXPECT_EQ(half*p, GenPoint(-std::sqrt(2)/2.0, std::sqrt(2)/2.0,0));
    EXPECT_EQ(full*p, GenPoint(-1,0,0));
}

TEST(MatTests, Chaining){
    auto p = GenPoint(1,0,1);
    auto A = MatRotateX(pi/2.0);
    auto B = MatScaling(5,5,5);
    auto C = MatTranslation(10,5,7);
    std::vector<Matrix> ops = {A,B,C};
    auto agg = Chain(ops);
    EXPECT_EQ(agg*p,GenPoint(15,0,7));
}