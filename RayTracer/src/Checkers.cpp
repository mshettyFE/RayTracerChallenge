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
    int x = floor(pt[0]);
    int y = floor(pt[1]);
    int z = floor(pt[2]);
    if(((static_cast<int>(x+y+z))%2 ) == 0){
        return color_a;
    }
    return color_b;
}