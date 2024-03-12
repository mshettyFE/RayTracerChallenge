#include "Shape.h"
#include "Matrix.h"
#include <stdexcept>

Shape::Shape(Matrix a_Transformation, Material material, std::shared_ptr<Shape> parent,std::string name){
    if (a_Transformation.get_dim() != 4){
        throw std::invalid_argument("Invalid Transformation Matrix");
    }
    mat = material;
    this->parent = parent;
    id = GlobalID;
    GlobalID += 1;
    this->name = name;
    Transformation = a_Transformation;
}

void Shape::set_nst(NestedShapeType new_nst){
    this->nested_shape_type  = new_nst;
}
NestedShapeType Shape::get_nst() const{
    return this->nested_shape_type;
}


void Shape::set_transform(Matrix a_Tranformation){
    this->Transformation = a_Tranformation;
}

Matrix Shape::get_only_this_transform() const{
    return Transformation;
}


Matrix Shape::get_transform(int count, bool verbose) const{
    Matrix output;
    if(verbose){
        if(get_parent() == nullptr){
            std::cout << "Level: " <<count << " Current: " << this->get_id() <<   " Parent: NULL" << std::endl;
        }
        else{
            std::cout << "Level: " <<count << " Current: " << this->get_id() <<  " Parent: " << this->get_parent()->get_id() << std::endl;
        }
    }
    if(get_parent() == nullptr){
        output = Transformation;
    }
    else{
        count++;
        Matrix parent_matrix = get_parent()->get_transform(count,verbose);
        output = parent_matrix*this->Transformation;
    }
    return output;
}

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

void Shape::print(unsigned int indent) const{
    auto indentation = std::string(indent,'\t');
    if(parent == nullptr){
        std::cout << indentation <<"Shape: " <<  name << " ID: " << get_id() << " Parent: NULL" << std::endl;
    }
    else{
        std::cout << indentation <<"Shape: " <<  name << " ID: " << get_id() << " Parent " << parent->get_id() <<  std::endl;
    }
}

Tuple Shape::normal_at(const Tuple& world_pt) const{
    Tuple local_point = get_transform().Inverse()*world_pt;
    local_point.set_type(TupType::POINT);
    Tuple local_normal  = local_normal_at(local_point);
    Tuple output = get_transform().Inverse().Transpose()*local_normal;
    output.set_type(TupType::VECTOR);
    output.normalize();
    return output;
}

std::vector<Impact> Shape::intersect(const Ray &other) const{
    Ray TransformedRay = other.transform(get_transform().Inverse());
    return local_intersect(TransformedRay);
}