#ifndef AABB_H
#define AABB_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Impact.h"
#include <memory>
#include <map>
#include <type_traits>

class AABB{
private:
    const Shape* enclosed_real_shape=nullptr;
    AABB* left=nullptr;
    AABB* right=nullptr;
    std::vector<double> min_bounds={0,0,0};
    std::vector<double> max_bounds={0,0,0};
public:
    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const{throw std::invalid_argument("AABB Doesn't have a normal vector.");};
    std::vector<Impact> local_intersect(const Ray &other) const{throw std::invalid_argument("TBD. Need to implement recursive bound checking");}; // Modify Cube intersection to work with AABB dimensions
    AABB(std::vector<double> min_bounds, std::vector<double> max_bounds){};
    std::unique_ptr<AABB> bound() const{return nullptr;};
};

#endif