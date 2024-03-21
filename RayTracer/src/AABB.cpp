#include "AABB.h"
#include <set>

AABB::AABB(std::initializer_list<double> min_bounds, std::initializer_list<double> max_bounds){
    if(min_bounds.size() != 3){throw std::invalid_argument("min_bounds must have a length of 3");}
    if(max_bounds.size() != 3){throw std::invalid_argument("max_bounds must have a length of 3");}
    std::vector<double> min_temp(min_bounds);
    std::vector<double> max_temp(max_bounds);
    this->min_bounds[0] = min_temp[0];
    this->min_bounds[1] = min_temp[1];
    this->min_bounds[2] = min_temp[2];
    this->max_bounds[0] = max_temp[0];
    this->max_bounds[1] = max_temp[1];
    this->max_bounds[2] = max_temp[2];
}

bool AABB::operator==(const AABB& other) const{
    if(other.enclosed_shapes.size() != this->enclosed_shapes.size()){return false;}
    for(int i=0; i< other.enclosed_shapes.size(); ++i){
        if(other.enclosed_shapes[i] != enclosed_shapes[i] ){
            return false;
        }
    }
    if(left!= other.left){return false;}
    if(right != other.right){return false;}
    if(std::abs(other.min_bounds[0]- min_bounds[0]) > glob_resolution){return false;}
    if(std::abs(other.min_bounds[1]- min_bounds[1]) > glob_resolution){return false;}
    if(std::abs(other.min_bounds[2]- min_bounds[2]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[0]- max_bounds[0]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[1]- max_bounds[1]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[2]- max_bounds[2]) > glob_resolution){return false;}
    return true;
}

bool AABB::operator!=(const AABB& other) const{
    return !(*this == other);
}


void AABB::print() const{
    std::set<const AABB*> visited;
    indented_print();
}

void AABB::indented_print(int indent) const{
    auto indentation = std::string(indent,'\t');
    std::cout << indentation << min_bounds[0] << " " << min_bounds[1] << " "<< min_bounds[2] << std::endl;
    std::cout << indentation << max_bounds[0] << " " << max_bounds[1] << " "<< max_bounds[2] << std::endl;
    for(const Shape* shp : enclosed_shapes){
        std::cout << indentation << shp << std::endl; 
    }
    indent++;
    if(left!= nullptr){
        left->indented_print(indent);
    }
    if(right!=nullptr){
        right->indented_print(indent);
    }
}

void AABB::set_min_x(double value) { min_bounds[0] = value;}
void AABB::set_min_y(double value) { min_bounds[1] = value;}
void AABB::set_min_z(double value) { min_bounds[2]= value;}
void AABB::set_max_x(double value) { max_bounds[0]= value;}
void AABB::set_max_y(double value) { max_bounds[1]= value;}
void AABB::set_max_z(double value) { max_bounds[2]= value;}


void AABB::add_point(const Tuple new_point){
    if(new_point.type() != TupType::POINT){
        throw std::invalid_argument("can't add vector to bounding box");
    }
    double x =  new_point.get_x();
    double y =  new_point.get_y();
    double z =  new_point.get_z();

    if(x <this->get_min_x()){set_min_x(x);}
    if(y <this->get_min_y()){set_min_y(y);}
    if(z <this->get_min_z()){set_min_z(z);}

    if(x >this->get_max_x()){set_max_x(x);}
    if(y >this->get_max_y()){set_max_y(y);}
    if(z >this->get_max_z()){set_max_z(z);}
}


void AABB::expand_box(const AABB* new_box){
    
}
