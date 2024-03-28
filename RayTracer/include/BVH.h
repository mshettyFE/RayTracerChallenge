#ifndef BVH_H
#define BVH_H

#include <vector>

#include "AABB.h"
#include "Ray.h"

class BVH{
private:
    std::unique_ptr<AABB> head=nullptr;
    void private_intersect(const Ray& r, const AABB* current_node, std::vector<Impact>& out_vector  ,unsigned long depth=0) const;
    unsigned long private_count_nodes (const AABB* node) const;
    void private_count_leaves (const AABB* node, unsigned long& tally) const;
public:
    BVH(const std::vector<std::unique_ptr<Shape>>& shapes);
    unsigned long count_nodes() const;
    unsigned long count_leaves() const;
    std::vector<Impact> intersect(const Ray& r) const;
    void print(bool verbose=false) const;
};

#endif