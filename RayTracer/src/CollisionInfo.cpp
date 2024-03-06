#include "Ray.h"
#include <stdexcept>
#include "Impact.h"
#include "CollisionInfo.h"
#include "Constants.h"

CollisionInfo::CollisionInfo(const Impact& impact, const Ray& ray, double resolution){
    this->impact = impact;
    this->pnt =  ray.position(impact.get_t());
    this->eye = -1.0*ray.get_direction();
    this->normal = impact.get_obj()->normal_at(pnt);
    if(eye.dot(normal) < 0){
        this->inside = true;
        this->normal = -1.0*this->normal;
    }
    else{
        this->inside = false;
    }
    this->over_pnt = this->pnt+this->normal*resolution;
}

Impact CollisionInfo::get_impact() const{return impact;};
Tuple CollisionInfo::get_pnt() const{return pnt;};
Tuple CollisionInfo::get_over_pnt() const{return over_pnt;};
Tuple CollisionInfo::get_eye() const{return eye;};
Tuple CollisionInfo::get_normal() const{return normal;};
bool CollisionInfo::is_inside() const{return inside;};

std::ostream& operator << (std::ostream &out, const CollisionInfo& other){
    out <<"Collision:";
    out << "\n\tImpact Info: " << other.get_impact() <<  "\t\nPosition: " << other.get_pnt() << "\tEye: " << other.get_eye();
    out << "\tNormal: " << other.get_normal() << "\tInside: " << other.is_inside() << std::endl; 
    return out;
}