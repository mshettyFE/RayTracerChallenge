#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Matrix.h"

TEST(Matrix, Initialization){
    Matrix a(3);
    std::cout << a << std::endl;
    // remember that you are filling the matrix up column-wise
    Matrix b(4, {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    std::cout << b << std::endl;
}