#include "Shape.h"
#include "Matrix.h"
#include <stdexcept>

Shape::Shape(Matrix a_Transformation, Material material){
    if (a_Transformation.get_dim() != 4){
        throw std::invalid_argument("Invalid Transformation Matrix");
    }
    Transformation = a_Transformation;
    mat = material;
    id = GlobalID;
    GlobalID += 1;
}

void Shape::set_transform(Matrix a_Tranformation){
    Transformation = a_Tranformation;
}

Matrix Shape::get_transform() const{return Transformation;}

Material Shape::get_material() const{return mat;}
unsigned long Shape::get_id() const{return id;}
