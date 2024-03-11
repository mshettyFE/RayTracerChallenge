#include "Shape.h"
#include "Matrix.h"
#include <stdexcept>

Shape::Shape(Matrix a_Transformation, Material material, std::shared_ptr<Shape> parent,std::string name){
    if (a_Transformation.get_dim() != 4){
        throw std::invalid_argument("Invalid Transformation Matrix");
    }
    Transformation = a_Transformation;
    mat = material;
    this->parent = parent;
    id = GlobalID;
    GlobalID += 1;
    this->name = name;
}

void Shape::set_nst(NestedShapeType new_nst){
    this->nested_shape_type  = new_nst;
}
NestedShapeType Shape::get_nst() const{
    return this->nested_shape_type;
}


void Shape::set_transform(Matrix a_Tranformation){
    Transformation = a_Tranformation;
}

Matrix Shape::get_transform() const{return Transformation;}

void Shape::set_material(Material new_mat){
    this->mat = new_mat;
}

Material Shape::get_material() const{return mat;}

std::shared_ptr<Shape> Shape::get_parent() const{
    return this->parent;
}
void Shape::set_parent(std::shared_ptr<Shape> new_parent){
    this->parent = new_parent;
}

unsigned long Shape::get_id() const{return id;}

std::string Shape::get_name() const{
    return name;
}

void Shape::print(unsigned int indent){
    auto indentation = std::string(indent,'\t');
    std::cout << indentation << "Shape: " << name  << " ID: " << id << std::endl;
}