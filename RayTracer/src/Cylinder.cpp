#include "Cylinder.h"

Cylinder::Cylinder(Matrix Transformation, Material material,
     double min, double max,bool closed ){
    this->set_transform(Transformation);
    mat = material;
    this->minimum = min;
    this->maximum = max;
    this->closed = closed;
}

void Cylinder::print() const{
    std::cout << "Cylinder" << std::endl;
    std::cout << this->get_transform() << std::endl;
    std::cout << mat;
    std::cout << "Min:" << minimum << "Max: " << maximum << "Closed: " << closed << std::endl;
}

std::vector<double> Cylinder::intersect(const Ray &other) const {
    double origin_x =  other.get_origin()[0];
    double origin_z =  other.get_origin()[2];
    double direction_x = other.get_direction()[0];
    double direction_z = other.get_direction()[2];
    double a = direction_x*direction_x+direction_z*direction_z;
    double b = (origin_x*direction_x+origin_z*direction_z)*2.0;
    double c = origin_x*origin_x+origin_z*origin_z-1;
    double disc = b*b-4*a*c;
    if(disc<0){return {};}
    if(a< glob_resolution){
        return {};
    }
    double add_on = std::sqrt(disc);
    double t0 = (-b-add_on)/(2.0*a);
    double t1 = (-b+add_on)/(2.0*a);
    return {t0,t1};
}

Tuple Cylinder::normal_at(const Tuple& pt) const {
    if(pt.get_dim() != 3){
        throw std::invalid_argument("pt must have dimension of 3 in Cylinder");
    }
    Tuple out({pt[0],0,pt[2]});
    out.normalize();
    return out;
}