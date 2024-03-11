#include "Pattern.h"
#include "Matrix.h"
#include "Shape.h"

Pattern::Pattern(Matrix trans, std::string name){
    transformation = trans;
    this->name = name;
}

void Pattern::set_transformation(const Matrix& new_mat){
    transformation = new_mat;
}

std::string Pattern::get_name() const{
    return name;
}

Color Pattern::at_object(const Tuple& world_pt, const std::shared_ptr<Shape>& obj) const{
    if(world_pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Stripes");
    }
    Tuple obj_pt = obj->get_transform().Inverse()*world_pt;
    Tuple pattern_pt = transformation.Inverse()*obj_pt;
    return at(pattern_pt);
}
