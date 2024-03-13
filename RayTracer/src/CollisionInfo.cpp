#include "Ray.h"
#include <stdexcept>
#include "CollisionInfo.h"
#include "Constants.h"
#include "Impact.h"
#include <map>
#include <algorithm>

CollisionInfo::CollisionInfo(const Impact& impt, const Ray& ray, const std::vector<Impact>& all_hits){
    this->impact = impt;
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
    this->reflect_v = ray.get_direction().reflect(this->normal);
    this->over_pnt = this->pnt+this->normal*glob_resolution;
    this->under_pnt = this->pnt-this->normal*glob_resolution;

    if(all_hits.size() ==0){
        std::vector<Impact> xs;
        xs.push_back(this->impact);
        calculate_indices(xs);
    }
    else{
        calculate_indices(all_hits);
    }
}

void CollisionInfo::calculate_indices(const std::vector<Impact>& all_hits){
// assumes all_hits is non-empty
    std::vector<const Shape*> containers;
    for(const Impact impt: all_hits){
// find n1
        if(this->impact == impt){
            if(containers.size() == 0){
                this->n1 = 1.0;
            }
            else{
                this->n1 = containers[containers.size()-1]->get_material().get_refractive_index();
            }
        }
// check if we have already entered the object before
        const Shape* current_obj = impt.get_obj();
        bool found = false;
        int offset = 0;
        for(auto obj: containers){
            if(obj->get_id() == current_obj->get_id()){
                found = true;
                break;
            }
            offset += 1;
        }
// we have entered the object before. Need to remove from containers
        if(found){
            containers.erase(containers.begin()+offset);
        }
// The ray has never encountered this object, and thus needs to add to end of containers
        else{
            containers.push_back(current_obj);
        }
// finding n2
        if(this->impact == impt){
            if(containers.size() == 0){
                this->n2 = 1.0;
            }
            else{
                this->n2 = containers[containers.size()-1]->get_material().get_refractive_index();
            }
// found second index, so return early.
            return;
        }
    }
// n2 is set to 1 implicitly here.
}


Impact CollisionInfo::get_impact() const{return impact;}
Tuple CollisionInfo::get_pnt() const{return pnt;}
Tuple CollisionInfo::get_over_pnt() const{return over_pnt;}
Tuple CollisionInfo::get_under_pnt() const{return under_pnt;}
Tuple CollisionInfo::get_eye() const{return eye;}
Tuple CollisionInfo::get_normal() const{return normal;}
Tuple CollisionInfo::get_reflect() const{return reflect_v;}


bool CollisionInfo::is_inside() const{return inside;};

double CollisionInfo::get_n1() const{return n1;}
double CollisionInfo::get_n2() const{return n2;}

double CollisionInfo::schlick() const{
// https://www.researchgate.net/publication/228959915_Reflections_and_Refractions_in_Ray_Tracing
    double cosine = eye.dot(normal);
    if(n1>n2){
        double n = n1/n2;
        double sin2_theta = n*n*(1-cosine*cosine);
        if(sin2_theta > 1.0){
            return 1.0;
        }
        double cos_theta = std::sqrt(1.0-sin2_theta);
        cosine = cos_theta;
    }
    double r0 = std::pow((n1-n2)/(n1+n2),2.0);
    return r0+(1.0-r0)*std::pow((1.0-cosine),5.0);
}

std::ostream& operator << (std::ostream &out, const CollisionInfo& other){
    out <<"Collision:";
    out << "\n\tImpact Info: " << other.get_impact() <<  "\t\nPosition: " << other.get_pnt() << "\tEye: " << other.get_eye();
    out << "\tNormal: " << other.get_normal() << "\tInside: " << other.is_inside() << std::endl; 
    return out;
}