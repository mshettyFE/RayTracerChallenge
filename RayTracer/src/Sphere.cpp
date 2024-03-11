#include "Sphere.h"
#include <iostream>
#include <cmath>

Sphere::Sphere(Matrix Transformation, Material material, std::shared_ptr<Shape> parent) : Shape(Transformation, material, parent, "Sphere"){}


Tuple Sphere::local_normal_at(const Tuple& pt) const{
// assumes pt already normalized
    return pt;
}

std::vector<Impact> Sphere::intersect(const Ray &other) const{
    std::vector<Impact> output;
    Ray TransformedRay = other.transform(get_transform().Inverse());
    Tuple origin_vec = TransformedRay.get_origin();
    origin_vec.set_type(TupType::VECTOR);
    Tuple dir = TransformedRay.get_direction();
    double a = dir.dot(dir);
    double b = 2.0*origin_vec.dot(dir);
    double c = origin_vec.dot(origin_vec)-1;
    double discriminant = b*b-4.0*a*c;
    // no intersection found
    if(discriminant<0){
        return output;
    }
    auto self = std::make_shared<Sphere>(*this);
    double t1 = (-b-std::sqrt(discriminant))/(2.0*a);
    double t2 = (-b+std::sqrt(discriminant))/(2.0*a);
    output.push_back(Impact(t1,self));
    output.push_back(Impact(t2,self));
    return output;
}

void Sphere::verbose_print() const{
    std::cout << name << std::endl;
    std::cout << "Transformation:" << std::endl;
    std::cout << get_transform() << std::endl;
    std::cout << mat << std::endl;
    std::cout << "Shape ID: " << id << std::endl;
}

Sphere glass_sphere(double IOR){
    Material mat(WHITE,0,1.0,IOR);
    Sphere s(MatIdentity(4), mat);
    return s;
}