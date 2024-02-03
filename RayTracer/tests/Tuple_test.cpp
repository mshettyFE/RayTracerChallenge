#include <gtest/gtest.h>
#include <iostream>
#include "Tuple.h"

TEST(TupleTests, PointAndVector) {
  Tuple a({1,2,3},1E-3,VType::POINT);
  Tuple b({1,2,3},1E-3,VType::VECTOR);
  ASSERT_NE(a,b);
}
