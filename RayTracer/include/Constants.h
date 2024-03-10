#ifndef CONSTANTS_H
#define CONSTANTS_H
// Named enum to delineate between Points and Vectors in homogeneous transformations
// w component choosen such that transformations become nice
#include <cmath>
enum TupType{
  VECTOR = 0, // corresponds to w component (ie. last component) of 0
  POINT = 1, // corresponds to w component  of 1
};

const double glob_resolution = 1E-3;
const double pi = std::acos(-1);

// index of refractions
const double VACUUM = 1.0;
const double AIR = 1.00029;
const double ICE =  1.31;
const double WATER = 1.33;
const double ACETONE = 1.36;
const double ALCOHOL = 1.38;
const double  FLUORITE = 1.433;
const double QUARTZ = 1.46;
const double GLYCERINE = 1.473;
const double  CROWN_GLASS = 1.52;
const double POLYSTYRENE = 1.57;
const double SAPPHIRE = 1.77;
const double DIAMOND = 2.4147;
#endif