#include "TestPattern.h"

TestPattern::TestPattern(Matrix trans){
    this->transformation = trans;
}
Color TestPattern::at(const Tuple& pt) const {
    return Color({pt[0],pt[1],pt[2]});
}
