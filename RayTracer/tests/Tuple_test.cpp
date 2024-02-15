#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Tuple.h"

TEST(TupleTests, PointAndVector) {
  Tuple a({1,2,3},TupType::POINT);
  Tuple b({1,2,3},TupType::VECTOR);
  EXPECT_NE(a,b);
  EXPECT_EQ(a.type(), TupType::POINT);
  EXPECT_EQ(b.type(), TupType::VECTOR);
}

TEST(TupleTests, TupleArithmetic){
  Tuple a({3,-2,5,1}, TupType::POINT);
  Tuple b({-2,3,1,0}, TupType::VECTOR);
  Tuple c({1,1,6,1}, TupType::POINT);
  EXPECT_EQ(a+b,c);
  c = a+b;
  EXPECT_EQ(c.type(), TupType::POINT);
  a = Tuple({3,-2,5,1}, TupType::VECTOR);
  c = Tuple({1,1,6,1}, TupType::VECTOR);
  a += b;
  EXPECT_EQ(a,c);
  EXPECT_EQ(a.type(), TupType::VECTOR);

  Tuple d({3.1,2.1,1.1}, TupType::POINT);
  Tuple og({3.1,2.1,1.1}, TupType::POINT);
  Tuple e({5.1,6.1,7.1}, TupType::VECTOR);
  Tuple f({-2,-4,-6}, TupType::POINT);
  EXPECT_EQ(d-e,f);
  EXPECT_EQ(d,og);
  f = d-e;
  EXPECT_EQ(f.type(), TupType::POINT);
  d  = Tuple({3.1,2.1,1.1}, TupType::VECTOR);
  e = Tuple({5.1,6.1,7.1}, TupType::VECTOR);
  f = Tuple({-2,-4,-6}, TupType::VECTOR);
  d -= e;
  EXPECT_EQ(d,f);
  EXPECT_EQ(d.type(), TupType::VECTOR);

  Tuple g({1,2,3}, TupType::POINT);
  Tuple h({2,4,6}, TupType::POINT);
  EXPECT_EQ(g*2.0,h);
  EXPECT_EQ(2.0*g,h);
  g *= 2.0;
  EXPECT_EQ(g,h);

  Tuple i({1,2,3},TupType::POINT);
  Tuple neg_i({-1,-2,-3},TupType::POINT);
  EXPECT_EQ(-i,neg_i);

  Tuple first({1,2,3}, TupType::VECTOR);
  Tuple second({4,5,6}, TupType::VECTOR);
  EXPECT_DOUBLE_EQ(first.dot(second),4+10+18);

  Tuple index_finger({1,2,3});
  Tuple middle_finger({2,3,4});
  Tuple thumb({-1,2,-1});
  EXPECT_EQ(index_finger.cross(middle_finger), thumb);
  EXPECT_EQ(middle_finger.cross(index_finger), -thumb);

}

TEST(TupleTests, Norms){
  Tuple a({1,0,1}, TupType::POINT);
  a.normalize();
  double sqrt_two = pow(2,0.5);
  Tuple b({1.0/sqrt_two,0,1.0/sqrt_two}, TupType::POINT);
  EXPECT_EQ(a,b);
}

TEST(TupleTests, Accessing){
    Tuple a({1,2,3});
    EXPECT_EQ(a[0],1);
    EXPECT_EQ(a[1],2);
    EXPECT_EQ(a[2],3);
    EXPECT_THROW({
        try{
            a[-1];
        }
        catch(const std::invalid_argument &e){
            EXPECT_STREQ( "Argument below 0", e.what() );
            throw;
        }
    }
    ,std::invalid_argument);
    EXPECT_THROW({
        try{
            a[5];
        }
        catch(const std::invalid_argument &e){
            EXPECT_STREQ( "Argument above 2", e.what() );
            throw;
        }
    }
    ,std::invalid_argument);
}

TEST(TupleTests, TupleCopyMove){
  Tuple a({1,0,1}, TupType::POINT);
  Tuple b = a;
  EXPECT_EQ(a,b);
}

TEST(TupleTests, Reflections){
  Tuple v({1,-1,0});
  Tuple n({0,1,0});
  Tuple e({1,1,0});
  Tuple o = v.reflect(n);
  EXPECT_EQ(e,o);
  v = Tuple({0,-1,0});
  n = Tuple({std::sqrt(2)/2.0,std::sqrt(2)/2.0,0});
  e = Tuple({1,0,0});
  o = v.reflect(n);
  EXPECT_EQ(e,o);

}