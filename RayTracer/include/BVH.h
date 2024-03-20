#ifndef BVH_H
#define BVH_H

#include <vector>

#include "AABB.h"

class BVH{
private:
    const AABB* head=nullptr;
public:
    BVH(std::vector<const AABB*> bounding_boxes);
    void print() const;
};

#endif