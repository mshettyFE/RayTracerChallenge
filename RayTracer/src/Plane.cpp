#include "Plane.h"
#include <stdexcept>

Plane::Plane( Matrix Transformation, Material material) : Shape(Transformation, material){
    normal_vector = Tuple({0,1,0},TupType::VECTOR);
}


Tuple Plane::normal_at(const Tuple& pt) const {
    return normal_vector;
}

std::vector<double> Plane::intersect(const Ray &other) const{
    std::vector<double> out;
    if(std::abs(other.get_direction()[1]) < glob_resolution){
        return out;
    }
    Ray TransformedRay = other.transform(this->get_transform().Inverse());
    Tuple origin_vec = TransformedRay.get_origin();
    double t = - TransformedRay.get_origin()[1] / TransformedRay.get_direction()[1];
    out.push_back(t);
    return out;
}

void Plane::print() const{
    std::cout << "Plane" << std::endl;
    std::cout << "Transformation:" << std::endl;
    std::cout << Transformation << std::endl;
    std::cout << mat << std::endl;
    std::cout << "Normal; " << normal_vector << std::endl;
    std::cout << "\tShape ID: " << id << std::endl;
}
