#include "Pattern.h"
#include "Cube.h"
#include "Matrix.h"
#include "Material.h"
#include <limits>
#include <algorithm>

Cube::Cube(Matrix Transformation, Material material) : Shape(Transformation, material){}

std::vector<double> Cube::check_axis(double origin, double direction) const{
    double tmin_num = (-1-origin);
    double tmax_num = (1-origin);
    double tmin, tmax;
    if(std::abs(direction) >= glob_resolution){
        tmin = tmin_num/direction;
        tmax = tmax_num/direction;
    }
    else{
        tmin = tmin_num*std::numeric_limits<double>::infinity();
        tmax = tmax_num*std::numeric_limits<double>::infinity();
    }
    if(tmin>tmax){
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    return {tmin,tmax};
}


Tuple Cube::normal_at(const Tuple& pt) const{
    if(pt.get_dim() != 3){
        throw std::invalid_argument("pt must have dimension of 3");
    }
    double max = -1.0*std::numeric_limits<double>::infinity();
    for(int i=0; i< 3; ++i){
        if(std::abs(pt[i]) > max){
            max = std::abs(pt[i]);
        }
    }
    if(max-std::abs(pt[0]) < glob_resolution){return Tuple({pt[0],0,0});}
    if(max-std::abs(pt[1]) < glob_resolution){return Tuple({0,pt[1],0});}
    return Tuple({0,0,pt[2]});
}

void Cube::print() const {
    std::cout << "Cube" << std::endl;
    std::cout << this->get_id() << std::endl;
    std::cout << this->get_transform() << std::endl;
    std::cout << this->get_material() << std::endl;
}

std::vector<double> Cube::intersect(const Ray &other) const {
    std::vector<double> minima;
    std::vector<double> maxima;
    auto bounds = check_axis(other.get_origin()[0],other.get_direction()[0]);
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    bounds = check_axis(other.get_origin()[1],other.get_direction()[1]);
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    bounds = check_axis(other.get_origin()[2],other.get_direction()[2]);
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    double largest_min = -1.0*std::numeric_limits<double>::infinity();
    for(auto x: minima){
        if(x>largest_min){
            largest_min = x;
        }    
    }
    double smallest_max = std::numeric_limits<double>::infinity();
    for(auto x: maxima){
        if(x<smallest_max){
            smallest_max = x;
        }    
    }
    if(largest_min > smallest_max){
        return {};
    }
    return  {largest_min,smallest_max};
}
