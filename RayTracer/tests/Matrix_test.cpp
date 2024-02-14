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
    
    Matrix id = MatIdentity(3);
    Matrix TruthID(3,{{1,0,0},{0,1,0},{0,0,1}});
    EXPECT_EQ(id,TruthID);
}

TEST(Matrix, TransInverse){
    Matrix cand(2,{{0,1},{2,0}});
    Matrix res(2,{{0,2},{1,0}});
    EXPECT_EQ(cand.Transpose(), res);
    cand.swap_row(0, 1);
    Matrix swapped(2,{{2,0},{0,1}});
    EXPECT_EQ(cand,swapped);
    cand.scale(0,10);
    Matrix scaled(2,{{20,0},{0,1}});
    EXPECT_EQ(cand,scaled);
    Matrix cand_mat(3,{{1,2,3},{3,2,1},{2,1,3}});
    Matrix invs = cand_mat.Inverse();
    Matrix true_invs(3,{{-5,3,4},{7,3,-8},{1,-3,4}});
    true_invs = (1.0/12.0)* true_invs;
    Matrix cand_mat_needs_swap(3,{{0,2,3},{3,2,1},{2,1,3}});
    Matrix invs_swap =cand_mat_needs_swap.Inverse();
    Matrix true_invs_swap(3,{{-5,3,4},{7,6,-9},{1,-4,6}});
    true_invs_swap = (1.0/17.0)*true_invs_swap;
    EXPECT_EQ(true_invs_swap,invs_swap);

    EXPECT_EQ(invs,true_invs);
}

TEST(Matrix, Transformations){
    Matrix trans = MatTranslation(5,-3,2);
    Tuple start({-3,4,5}, TupType::POINT);
    Tuple end({2,1,7}, TupType::POINT);
    Tuple inv_end({-8,7,3}, TupType::POINT);
    EXPECT_EQ(trans*start,end);
    Matrix inv_trans = trans.Inverse();
    EXPECT_EQ(inv_trans*start,inv_end);
    Tuple v({-3,4,5});
    EXPECT_EQ(trans*v,v);
    Matrix s = MatScaling(2,3,4);
    start = Tuple({-4,6,8}, TupType::POINT);
    EXPECT_EQ(s*start,Tuple({-8,18,32},TupType::POINT));
    start = Tuple({0,1,0}, TupType::POINT);
    Matrix RotX = MatRotateX(acos(-1)/2.0);
    EXPECT_EQ(RotX*start, Tuple({0,0,1},TupType::POINT));
}