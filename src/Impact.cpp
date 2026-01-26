#include "Impact.h"
#include "Shape.h"

Impact::Impact(double time,const Shape* obj, double u, double v){
    this->time  = time;
    this->obj = obj;
    this->u = u;
    this->v = v;
}

Impact::Impact(){
    this->time = -1;
    this->obj = nullptr;
    this->u = 0;
    this->v = 0;
}

double Impact::get_t() const{return time;}
const Shape* Impact::get_obj() const{return obj;}
double Impact::get_u() const{    return u;}
double  Impact::get_v() const{    return v;}


std::ostream& operator << (std::ostream &out, const Impact& other){
    out << "Time: " << other.get_t() << '\n';
    other.get_obj()->print();
    out << "U: " << other.get_u() << "V: " << other.get_v() << std::endl;
    return out;
}

bool Impact::operator==(const Impact& other) const{
    bool check = (this->get_obj()->get_id() == other.get_obj()->get_id()) && ((this->get_t()-other.get_t()) < glob_resolution) &&  ((this->get_v()-other.get_v()) < glob_resolution) &&  ((this->get_u()-other.get_u()) < glob_resolution);
    if( check){
        return true;
    }
    return false;
}
bool Impact::operator!=(const Impact& other) const{
    bool check = (this->get_obj()->get_id() == other.get_obj()->get_id()) && ((this->get_t()-other.get_t()) < glob_resolution) &&  ((this->get_v()-other.get_v()) < glob_resolution) &&  ((this->get_u()-other.get_u()) < glob_resolution);
    if(check){
        return false;
    }
    return true;
}

bool Impact::operator<(const Impact& other) const{
    return get_t() < other.get_t();
}

bool Impact::operator<=(const Impact& other) const{
    return get_t() <= other.get_t();
}

bool Impact::operator>(const Impact& other) const{
    return get_t() > other.get_t();
}

bool Impact::operator>=(const Impact& other) const{
    return get_t() >= other.get_t();
}


// helper function to get first hit with positive time. Return t=-1 if nothing found
Impact first_hit(std::vector<Impact> intersections){
    Impact out;
    for(auto const& hit: intersections){
        if(hit.get_t() > 0){
            return hit;
        }
    }
    return out;
}
