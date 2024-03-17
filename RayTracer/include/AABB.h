#ifndef AABB_H
#define AABB_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Group.h"
#include "Impact.h"
#include <memory>
#include <map>
#include <type_traits>

class AABB : public Group{
private:
    std::vector<const AABB*> sub_boxes;
    const Shape* enclosed_real_shape=nullptr;
    std::vector<double> min_bounds={0,0,0};
    std::vector<double> max_bounds={0,0,0};
    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override{throw std::invalid_argument("AABB Doesn't have a normal vector.");};
    std::vector<Impact> local_intersect(const Ray &other) const override{throw std::invalid_argument("TBD. Need to implement recursive bound checking");}; // Modify Cube intersection to work with AABB dimensions
public:
    void verbose_print() const override{};
    AABB(std::vector<double> min_bounds, std::vector<double> max_bounds, const Shape* enclosed_real_shape=nullptr) : Group(MatIdentity(4), Material(), nullptr){};
};

#endif