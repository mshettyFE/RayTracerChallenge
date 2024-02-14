#include "Sphere.h"
#include <iostream>

Tuple Sphere::normal_at(Tuple world_pt){
    Tuple obj_pt = Transformation.Inverse()*world_pt;
    Tuple obj_normal = obj_pt - Tuple({0,0,0}, TupType::POINT);
    Tuple world_normal = Transformation.Inverse().Transpose()* obj_normal;
    world_normal.set_type(TupType::VECTOR);
    return world_normal;
}

void Sphere::print(){
    std::cout << "REPLACE_ME" << std::endl;
}
