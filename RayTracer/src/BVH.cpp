#include "BVH.h"

BVH::BVH(const std::vector<std::unique_ptr<Shape>>& bounding_boxes){

}

void BVH::print() const{
    head->print();    
}