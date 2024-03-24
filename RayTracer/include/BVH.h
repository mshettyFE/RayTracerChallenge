#ifndef BVH_H
#define BVH_H

#include <vector>

#include "AABB.h"

class BVH{
private:
    std::unique_ptr<AABB> head=nullptr;
public:
    BVH(const std::vector<std::unique_ptr<Shape>>& shapes);
    const AABB* insert(std::unique_ptr<AABB>& new_box);
    void print() const;
};

#endif