#include "GradientPattern.h"
#include "Color.h"

GradientPattern::GradientPattern(Color lower, Color upper, Matrix transform) : Pattern(transform){
    this->lower = lower;
    this->upper = upper;
}

Color GradientPattern::at(const Tuple& pt) const{
    Color dist = upper-lower;
    double frac = pt[0]-floor(pt[0]);
    return this->lower+dist*frac;
}
