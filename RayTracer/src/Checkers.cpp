#include "Stripes.h"
#include "Constants.h"
#include "Shape.h"
#include "Pattern.h"
#include "Checkers.h"
#include <math.h>
#include <stdexcept>



Checkers::Checkers(const Color& a, const Color& b, Matrix trans) : Pattern(trans, "Checkers"){
    color_a = a;
    color_b = b;
}

Color Checkers::at(const Tuple& pt) const {
    if(pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Checkers");
    }
    if(static_cast<int>(floor(pt[0])+floor(pt[1])+floor(pt[2]))%2  == 0){
        return color_a;
    }
    return color_b;
}