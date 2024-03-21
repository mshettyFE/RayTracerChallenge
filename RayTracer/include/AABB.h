#ifndef AABB_H
#define AABB_H

#include "Ray.h"
#include "Impact.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include <map>
#include <type_traits>

class Shape;

class AABB{
private:
    std::vector<const Shape*> enclosed_shapes;
    const AABB* left=nullptr;
    const AABB* right=nullptr;
    std::array<double,3> min_bounds={INFTY,INFTY,INFTY};
    std::array<double,3> max_bounds={NEG_INFTY,NEG_INFTY,NEG_INFTY};
    void indented_print(int indent=0) const;


public:
    std::vector<Impact> local_intersect(const Ray &other) const{throw std::invalid_argument("TBD. Need to implement recursive bound checking");}; // Modify Cube intersection to work with AABB dimensions
    AABB(std::initializer_list<double> min_bounds, std::initializer_list<double> max_bounds);
    AABB(){}

    void add_point(const Tuple new_point);

    double get_min_x() const{return min_bounds[0];}
    double get_min_y() const{return min_bounds[1];}
    double get_min_z() const{return min_bounds[2];}
    double get_max_x() const{return max_bounds[0];}
    double get_max_y() const{return max_bounds[1];}
    double get_max_z() const{return max_bounds[2];}

    void set_min_x(double value) ;
    void set_min_y(double value) ;
    void set_min_z(double value) ;
    void set_max_x(double value) ;
    void set_max_y(double value) ;
    void set_max_z(double value) ;


    bool operator==(const AABB& other) const;
    bool operator!=(const AABB& other) const;

    void expand_box(const AABB* new_box);

    void print() const;
};

#endif