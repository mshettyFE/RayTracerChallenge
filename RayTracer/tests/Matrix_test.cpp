#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Matrix.h"
#include "Tuple.h"

TEST(Matrix, Initialization){
    Matrix a(3);
    Matrix b(3, {{0,0,0},{0,0,0},{0,0,0}});
    EXPECT_EQ(a,b);
    Matrix c(4, {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    std::cout << c << std::endl;
}

TEST(Matrix, Multiplication){
    Matrix small(2,{{1,2},{3,4}});
    Matrix mul(2,{{2,4},{6,8}});
    EXPECT_EQ(2*small,mul);
    EXPECT_EQ(small*2, mul);
    Matrix next(2,{{5,6},{7,8}});
    Matrix small_next(2,{{19,22},{43,50}});
    Matrix next_small(2,{{23,34},{31,46}});
    EXPECT_EQ(small*next, small_next);
    EXPECT_EQ(next*small, next_small);
    Tuple r({1}, TupType::POINT);
    Tuple res = small*r;
    Matrix id = MatIdentity(3);
    Matrix TruthID(3,{{1,0,0},{0,1,0},{0,0,1}});
    EXPECT_EQ(id,TruthID);
}

TEST(Matrix, TransInverse){
    Matrix cand(2,{{0,1},{2,0}});
    Matrix res(2,{{0,2},{1,0}});
    EXPECT_EQ(cand.Transpose(), res);
}