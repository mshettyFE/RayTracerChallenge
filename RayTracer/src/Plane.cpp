#include "Plane.h"
#include <stdexcept>

Plane::Plane( Matrix Transformation, Material material, Shape* parent) : Shape(Transformation, material, parent, "Plane"){
    normal_vector = Tuple({0,1,0},TupType::VECTOR);
}

Tuple Plane::local_normal_at(const Tuple& pt, const Impact& impt) const {
    return normal_vector;
}

std::vector<Impact> Plane::local_intersect(const Ray &other) const{
    std::vector<Impact> out;
    if(std::abs(other.get_direction()[1]) < glob_resolution){
        return out;
    }
    Tuple origin_vec = other.get_origin();
    double t = - origin_vec[1] / other.get_direction()[1];
    out.push_back(Impact(t, this));
    return out;
}

void Plane::verbose_print() const{
    std::cout << name << std::endl;
    std::cout << "Transformation:" << std::endl;
    std::cout << get_transform() << std::endl;
    std::cout << mat << std::endl;
    std::cout << "Normal; " << normal_vector << std::endl;
    std::cout << "\tShape ID: " << id << std::endl;
}
