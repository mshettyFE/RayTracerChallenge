#include "Sphere.h"
#include <iostream>
#include <cmath>

Sphere::Sphere(Matrix Transformation, Material material, std::shared_ptr<Shape> parent) : Shape(Transformation, material, parent){}


Tuple Sphere::normal_at(const Tuple& world_pt) const{
    Tuple obj = Transformation.Inverse()*world_pt; // Convert world point to object point
    obj.set_type(TupType::POINT); // Sphere's normal at a point on the sphere is in the same direction as the vector of that point
    Tuple world_normal = Transformation.Transpose().Inverse()* obj; 
    world_normal.set_type(TupType::VECTOR); // The type of the tuple can get corrupted when Doing 4x4 matrix math. Set to vector to restore.
    world_normal.normalize(); // Make a unit vector
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
    std::cout << "Sphere" << std::endl;
    std::cout << "Transformation:" << std::endl;
    std::cout << Transformation << std::endl;
    std::cout << mat << std::endl;
    std::cout << "\tShape ID: " << id << std::endl;
}

Sphere glass_sphere(double IOR){
    Material mat(WHITE,0,1.0,IOR);
    Sphere s(MatIdentity(4), mat);
    return s;
}