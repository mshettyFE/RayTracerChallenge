#include "Shape.h"
#include "Matrix.h"
#include <stdexcept>

unsigned long Shape::GlobalID = 0;

Shape::Shape(Matrix a_Transformation, Material material){
    if (a_Transformation.get_dim() != 4){
        throw std::invalid_argument("Invalid Transformation Matrix");
    }
    Transformation = a_Transformation;
    mat = material;
    id = ++GlobalID;
}

void Shape::set_transform(Matrix a_Tranformation){
    Transformation = a_Tranformation;
}

Material Shape::get_material() const{return mat;}
