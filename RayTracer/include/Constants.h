#ifndef CONSTANTS_H
#define CONSTANTS_H
// Named enum to delineate between Points and Vectors in homogeneous transformations
// w component choosen such that transformations become nice
enum TupType{
  VECTOR = 0, // corresponds to w component (ie. last component) of 0
  POINT = 1, // corresponds to w component  of 1
};

const double glob_resolution = 1E-6;
#endif