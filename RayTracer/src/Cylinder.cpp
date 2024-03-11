#include "Cylinder.h"

Cylinder::Cylinder(Matrix Transformation, Material material, std::shared_ptr<Shape> parent,
     double min, double max,bool closed ): Shape(Transformation,material,parent,"Cylinder"){
    this->minimum = min;
    this->maximum = max;
    this->closed = closed;
}

void Cylinder::verbose_print() const{
    std::cout << "Cylinder" << std::endl;
    std::cout << this->get_transform() << std::endl;
    std::cout << mat;
    std::cout << "Min:" << minimum << "Max: " << maximum << "Closed: " << closed << std::endl;
}

bool Cylinder::check_cap(const Ray& r, double time) const{
    auto origin = r.get_origin();
    auto direction = r.get_direction();
    double x = origin[0]+time*direction[0];
    double z = origin[2]+time*direction[2];
    return (x*x+z*z) <= 1;
}

std::vector<Impact> Cylinder::intersect_cap(const Ray& r) const{
    double dir_y = r.get_direction()[1];
    if(!(this->closed) || (std::abs(dir_y) < glob_resolution)){
        return {};        
    }
    std::vector<Impact> out;
    auto self = std::make_shared<Cylinder>(*this);
    double ori_y = r.get_origin()[1];
    double t = (minimum-ori_y)/dir_y;
    if(check_cap(r, t)){
        out.push_back(Impact(t,self));
    }
    t = (maximum-ori_y)/dir_y;
    if(check_cap(r, t)){
        out.push_back(Impact(t,self));
    }
    return out;
}

std::vector<Impact> Cylinder::intersect(const Ray &other) const {
    double origin_x =  other.get_origin()[0];
    double origin_y =  other.get_origin()[1];
    double origin_z =  other.get_origin()[2];
    double direction_x = other.get_direction()[0];
    double direction_y = other.get_direction()[1];
    double direction_z = other.get_direction()[2];
    double a = direction_x*direction_x+direction_z*direction_z;
    double b = (origin_x*direction_x+origin_z*direction_z)*2.0;
    double c = origin_x*origin_x+origin_z*origin_z-1;
    double disc = b*b-4*a*c;
    if(disc<0){return {};}

    std::vector<Impact> out;
    auto self = std::make_shared<Cylinder>(*this);
    if(a> glob_resolution){
        double add_on = std::sqrt(disc);
        double t0 = (-b-add_on)/(2.0*a);
        double t1 = (-b+add_on)/(2.0*a);
        if(t0 > t1){
            double temp = t0;
            t0 = t1;
            t1 = temp;
        }
        double y0 = origin_y+t0*direction_y;
        if((y0 > minimum) && (y0 < maximum)){
            out.push_back(Impact(t0,self));
        }
        double y1 = origin_y+t1*direction_y;
        if((y1 > minimum) && (y1 < maximum)){
            out.push_back(Impact(t1,self));
        }
    }
    for(auto hit: intersect_cap(other)){
        out.push_back(hit);
    }
    return out;
}

Tuple Cylinder::normal_at(const Tuple& pt) const {
    if(pt.get_dim() != 3){
        throw std::invalid_argument("pt must have dimension of 3 in Cylinder");
    }
    double dist = pt[0]*pt[0]+pt[2]*pt[2];
    if(dist <1){
        if(pt[1] >= maximum-glob_resolution){
            return Tuple ({0,1,0});
        }
        else if(pt[1] <= minimum+glob_resolution){
            return Tuple ({0,-1,0});        
        }
    }
    Tuple out({pt[0],0,pt[2]});
    out.normalize();
    return out;
}

void Cylinder::set_min(double val) {
    this->minimum = val;
}

void Cylinder::set_max(double val) {
    this->maximum = val;
}

void Cylinder::set_closed(bool val) {
    this->closed = val;
}

double Cylinder::get_min() const{return minimum;}
double  Cylinder::get_max() const{return maximum;}
bool  Cylinder::get_closed() const{return closed;}