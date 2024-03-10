#include "Shape.h"
#include "Tuple.h"
#include "Impact.h"
#include <memory>


Impact::Impact(double time,const std::shared_ptr<Shape>& obj){
    this->time  = time;
    this->obj = obj;
}

Impact::Impact(){
    this->time = -1;
    this->obj = nullptr;
}


double Impact::get_t() const{return time;}
std::shared_ptr<Shape> Impact::get_obj() const{return obj;}

std::ostream& operator << (std::ostream &out, const Impact& other){
    out << "Time: " << other.get_t() << '\n';
    other.get_obj()->print();
    return out;
}

bool Impact::operator==(const Impact& other) const{
    if( (this->get_obj()->get_id() == other.get_obj()->get_id()) && (this->get_t()-other.get_t()) < glob_resolution){
        return true;
    }
    return false;
}
bool Impact::operator!=(const Impact& other) const{
    if( (this->get_obj()->get_id() == other.get_obj()->get_id()) && (this->get_t()-other.get_t()) < glob_resolution){
        return false;
    }
    return true;
}


// helper function to get first hit with positive time. Return t=-1 if nothing found
Impact first_hit(std::vector<Impact> intersections){
    Impact out;
    for(auto hit: intersections){
        if(hit.get_t() > 0){
            out = hit;
        }
    }
    return out;
}
