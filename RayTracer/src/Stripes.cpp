#include "Stripes.h"
#include "Constants.h"
#include "Shape.h"
#include <math.h>
#include <stdexcept>

Stripes::Stripes(const Color& a, const Color& b, Matrix trans){
    this->color_a = a;
    this->color_b = b;
    transformation = trans;
}

Color Stripes::at(const Tuple& pt) const{
    if(pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Stripes");
    }
    if((static_cast<int>(floor(pt[0]))%2 ) == 0){
        return color_a;
    }
    return color_b;
}


Color Stripes::at_object(const Tuple& world_pt, const std::shared_ptr<Shape> obj) const{
    if(world_pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Stripes");
    }
    Tuple obj_pt = obj->get_transform().Inverse()*world_pt;
    Tuple pattern_pt = transformation.Inverse()*obj_pt;
    return at(pattern_pt);
}
