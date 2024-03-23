#include "BVH.h"
#include "Shape.h"

BVH::BVH(const std::vector<std::unique_ptr<Shape>>& shapes){
// define empty head
    this->head = std::make_unique<AABB>(std::move(AABB()));
    for(auto const& shape: shapes){
        auto current_bb = shape->bound();
    }
}

void BVH::print() const{
    head->print();    
}