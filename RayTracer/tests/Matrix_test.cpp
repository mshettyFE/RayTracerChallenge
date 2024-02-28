#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Matrix.h"
#include "Tuple.h"

TEST(MatrixTests, Initialization){
    Matrix a(3);
    Matrix b({{0,0,0},{0,0,0},{0,0,0}});
    EXPECT_EQ(a,b);
    Matrix c( {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
}

TEST(MatrixTests, Multiplication){
    Matrix small({{1,2},{3,4}});
    Matrix mul({{2,4},{6,8}});
    EXPECT_EQ(2*small,mul);
    EXPECT_EQ(small*2, mul);
    Matrix next({{5,6},{7,8}});
    Matrix small_next({{19,22},{43,50}});
    Matrix next_small({{23,34},{31,46}});
    EXPECT_EQ(small*next, small_next);
    EXPECT_EQ(next*small, next_small);
    
    Matrix id = MatIdentity(3);
    Matrix TruthID({{1,0,0},{0,1,0},{0,0,1}});
    EXPECT_EQ(id,TruthID);
}

TEST(MatrixTests, TransInverse){
    Matrix cand({{0,1},{2,0}});
    Matrix res({{0,2},{1,0}});
    EXPECT_EQ(cand.Transpose(), res);
    cand.swap_row(0, 1);
    Matrix swapped({{2,0},{0,1}});
    EXPECT_EQ(cand,swapped);
    cand.scale(0,10);
    Matrix scaled({{20,0},{0,1}});
    EXPECT_EQ(cand,scaled);
    Matrix cand_mat({{1,2,3},{3,2,1},{2,1,3}});
    Matrix invs = cand_mat.Inverse();
    Matrix true_invs({{-5,3,4},{7,3,-8},{1,-3,4}});
    true_invs = (1.0/12.0)* true_invs;
    Matrix cand_mat_needs_swap({{0,2,3},{3,2,1},{2,1,3}});
    Matrix invs_swap =cand_mat_needs_swap.Inverse();
    Matrix true_invs_swap({{-5,3,4},{7,6,-9},{1,-4,6}});
    true_invs_swap = (1.0/17.0)*true_invs_swap;
    EXPECT_EQ(true_invs_swap,invs_swap);

    EXPECT_EQ(invs,true_invs);
}

TEST(MatrixTests, Translations){
// normal
    Matrix trans = MatTranslation(5,-3,2);
    Tuple start({-3,4,5}, TupType::POINT);
    Tuple end({2,1,7}, TupType::POINT);
    EXPECT_EQ(trans*start,end);
// inverse
    Matrix inv_trans = trans.Inverse();
    Tuple inv_end({-8,7,3}, TupType::POINT);
    EXPECT_EQ(inv_trans*start,inv_end);
// shouldn't affect vectors though
    Tuple v({-3,4,5});
    EXPECT_EQ(trans*v,v);    
}

TEST(MatrixTests,Scaling){
    Matrix s = MatScaling(2,3,4);
// should work for points
    Tuple start = Tuple({-4,6,8}, TupType::POINT);
    EXPECT_EQ(s*start,Tuple({-8,18,32},TupType::POINT));
// and vectors
    start = Tuple({-4,6,8});
    EXPECT_EQ(s*start,Tuple({-8,18,32}));
// Inverse should work
    EXPECT_EQ((s.Inverse())*start,Tuple({-2,2,2}));
// Reflection is a special kind of scaling
    s = MatScaling(-1,1,1);
    start = Tuple({2,3,4}, TupType::POINT);
    EXPECT_EQ(s*start, Tuple({-2,3,4}, TupType::POINT));
}

TEST(MatrixTests, RotX){
    Tuple start = Tuple({0,1,0}, TupType::POINT);
    Matrix fourth = MatRotateX(pi/2.0);
    Matrix eighth = MatRotateX(pi/4.0);
    EXPECT_EQ(eighth*start, Tuple({0,std::sqrt(2)/2.0,std::sqrt(2)/2.0},TupType::POINT));
    EXPECT_EQ(fourth*start, Tuple({0,0,1},TupType::POINT));
    EXPECT_EQ(eighth.Inverse()*start, Tuple({0,std::sqrt(2)/2.0,-std::sqrt(2)/2.0},TupType::POINT));
}
TEST(MatrixTests, RotY){
    Tuple start = Tuple({0,0,1}, TupType::POINT);
    Matrix eighth = MatRotateY(pi/4.0);
    Matrix fourth = MatRotateY(pi/2.0);
    EXPECT_EQ(eighth*start, Tuple({std::sqrt(2)/2.0,0,std::sqrt(2)/2.0},TupType::POINT));
    EXPECT_EQ(fourth*start, Tuple({1,0,0},TupType::POINT));
}

TEST(MatrixTests, RotZ){
    Tuple start = Tuple({0,1,0}, TupType::POINT);
    Matrix fourth = MatRotateZ(pi/2.0);
    Matrix eighth = MatRotateZ(pi/4.0);
    EXPECT_EQ(eighth*start, Tuple({-std::sqrt(2)/2.0,std::sqrt(2)/2.0,0},TupType::POINT));
    EXPECT_EQ(fourth*start, Tuple({-1,0,0},TupType::POINT));
}

TEST(MatrixTests, Shearing){
    Matrix candidate = MatShear(1,0,0,0,0,0);
    Tuple p = Tuple({2,3,4},TupType::POINT);
    EXPECT_EQ(candidate*p, Tuple({5,3,4},TupType::POINT));
    candidate = MatShear(0,1,0,0,0,0);
    EXPECT_EQ(candidate*p, Tuple({6,3,4},TupType::POINT));
    candidate = MatShear(0,0,1,0,0,0);
    EXPECT_EQ(candidate*p, Tuple({2,5,4},TupType::POINT));
    candidate = MatShear(0,0,0,1,0,0);
    EXPECT_EQ(candidate*p, Tuple({2,7,4},TupType::POINT));
    candidate = MatShear(0,0,0,0,1,0);
    EXPECT_EQ(candidate*p, Tuple({2,3,6},TupType::POINT));
    candidate = MatShear(0,0,0,0,0,1);
    EXPECT_EQ(candidate*p, Tuple({2,3,7},TupType::POINT));
}

TEST(MatrixTests,Chaining){
    Matrix rotation = MatRotateX(pi/2.0);
    Matrix scaling = MatScaling(5,5,5);
    Matrix translation = MatTranslation(10,5,7);
    std::vector<Matrix> trans = {rotation, scaling, translation};
    Matrix chained = Chain(trans);
    Tuple p = Tuple({1,0,1}, TupType::POINT);
    EXPECT_EQ(chained*p, Tuple({15,0,7}, TupType::POINT));
}

