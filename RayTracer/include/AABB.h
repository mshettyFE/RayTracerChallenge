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
    bool operator==(const AABB& other) const;
    bool operator!=(const AABB& other) const;

    void add_child(const Shape* child){
        if(child!= nullptr){
            enclosed_shapes.push_back(child);
            return;
        }
        throw std::invalid_argument("Can't have nullptr as a child in AABB");
    }

    void print() const;
};

#endif