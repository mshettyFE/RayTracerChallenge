#include "Sphere.h"
#include <iostream>
#include <cmath>

Sphere::Sphere() : Shape() {}

Sphere::Sphere(Matrix Transformation) : Shape(Transformation){}


Tuple Sphere::normal_at(Tuple world_pt){
    Tuple obj = Transformation.Inverse()*world_pt;
    obj.set_type(TupType::POINT);
    Tuple world_normal = Transformation.Inverse().Transpose()* obj;
    world_normal.set_type(TupType::VECTOR);
    return world_normal;
}

std::vector<double> Sphere::intersect(const Ray &other) const{
    std::vector<double> output;
    Ray TransformedRay = other.transform(Transformation.Inverse());
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
    double t1 = (-b-std::sqrt(discriminant))/(2.0*a);
    double t2 = (-b+std::sqrt(discriminant))/(2.0*a);
    output.push_back(t1);
    output.push_back(t2);
    return output;
}


void Sphere::print() const{
    std::cout << Transformation << std::endl;
}

std::ostream& operator << (std::ostream &out, const Sphere& other){
    other.print();
    return out;
}
