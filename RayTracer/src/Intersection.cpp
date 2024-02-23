#include "Intersection.h"
#include <memory>
#include "Ray.h"
#include <stdexcept>
#include "Constants.h"

Intersection::Intersection(double time,const std::shared_ptr<Shape>& obj, const Ray& ray){
    this->time = time;
    this->obj = obj;
    this->pnt =  ray.position(time);
    this->eye = -ray.get_direction();
    this->normal = obj->normal_at(pnt);
    if(eye.dot(normal) < 0){
        inside = true;
        this->normal = -this->normal;
    }
    else{
        inside = false;
    }
}

double Intersection::get_t() const{return time;}
std::shared_ptr<Shape> Intersection::get_obj() const{return obj;};
Tuple Intersection::get_pnt() const{return pnt;};
Tuple Intersection::get_eye() const{return eye;};
Tuple Intersection::get_normal() const{return normal;};
bool Intersection::is_inside() const{return inside;};

std::ostream& operator << (std::ostream &out, const Intersection& other){
    out <<"Intersection:"<<  "\n\tTime: " << other.get_t() <<  "\t\nPosition: " << other.get_pnt() << "\tEye: " << other.get_eye();
    out << "\tNormal: " << other.get_normal() << "\tInside: " << other.is_inside() << std::endl; 
    return out;
}