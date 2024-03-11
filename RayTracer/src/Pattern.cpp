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

Matrix Pattern::get_transformation() const{
    return transformation;
}


std::string Pattern::get_name() const{
    return name;
}

Color Pattern::at_object(const Tuple& world_pt, const std::shared_ptr<Shape>& obj) const{
    if(world_pt.type() != TupType::POINT){
        throw std::invalid_argument("pt must be a point in Stripes");
    }
    Tuple pattern_pt = (obj->get_transform()*this->get_transformation()).Inverse()*world_pt;
    return at(pattern_pt);
}
