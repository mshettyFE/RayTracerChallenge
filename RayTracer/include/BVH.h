#ifndef BVH_H
#define BVH_H

#include <vector>

#include "AABB.h"

class BVH{
private:
    const AABB* head=nullptr;
public:
    BVH(const std::vector<std::unique_ptr<Shape>>& bounding_boxes);
    void print() const;
};

#endif