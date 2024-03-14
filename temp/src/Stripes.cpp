#include "Stripes.h"
#include "Constants.h"
#include "Shape.h"
#include <cmath>
#include <stdexcept>

Stripes::Stripes(const Color& a, const Color& b, Matrix trans) : Pattern(trans,"Stripes"){
    this->color_a = a;
    this->color_b = b;
}

Color Stripes::at(const Tuple& pt) const{
    if(pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Stripes");
    }
    if((static_cast<int>(std::round(pt[0]))%2 ) == 0){
        return color_a;
    }
    return color_b;
}