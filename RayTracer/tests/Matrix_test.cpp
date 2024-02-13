#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Matrix.h"

TEST(Matrix, Initialization){
    Matrix a(3);
    Matrix b(3, {{0,0,0},{0,0,0},{0,0,0}});
    EXPECT_EQ(a,b);
    Matrix c(4, {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    std::cout << c << std::endl;
}