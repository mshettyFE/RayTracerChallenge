#include "BVH.h"
#include "Shape.h"
#include <algorithm>

BVH::BVH(const std::vector<std::unique_ptr<Shape>>& shapes, unsigned int max_depth){
// define empty head
    this->head = std::make_unique<AABB>(std::move(AABB()));
    std::vector<std::unique_ptr<AABB>> boxes;
    int count = 0;
    for(auto const& shape: shapes){
        auto current_bb = shape->bound();
        Matrix new_mat = shape->get_aggregate_transform();
        current_bb->transform(new_mat);
        head->expand_box(current_bb.get());
        boxes.push_back(std::move(current_bb));
        count++;
    }
    for(int i=0; i<boxes.size(); ++i){
//        std::cout << "Shape Number " << i << std::endl;
        head->insert(boxes[i], 0, max_depth);
    }
}

void BVH::private_intersect(const Ray& r, const AABB* current_node, std::vector<Impact>& out_vector  ,unsigned long depth) const{
    // If the current node has a shape, then add the intersections of the associated shape
    if(current_node->get_shape()!=nullptr){
        auto hits = current_node->get_shape()->intersect(r);
        out_vector.insert(out_vector.end(),hits.begin(), hits.end());
    }
    depth++;
    // need to do central children first since it is ambiguous which side it falls in
    for(auto const& center_child: current_node->get_center()){
        if(center_child->intersect(r)){
            private_intersect(r,center_child,out_vector,  depth);
        }
    }
    // otherwise, recurse on left children, right children, and central children, and join vectors, then return output
    auto lft = current_node->get_left();
    auto rght = current_node->get_right();
// early return if left intersects
    if(lft!= nullptr){
        if(lft->intersect(r)){
            private_intersect(r,lft,out_vector, depth);
        }
    }
    if(rght!= nullptr){
        if(rght->intersect(r)){
            private_intersect(r,rght, out_vector, depth);
        }
    }
    return ;
}


std::vector<Impact> BVH::intersect(const Ray& r) const{
    std::vector<Impact> current_level_outputs;
    if(this->head->intersect(r)){ // check if the ray intersects the world. If it doesn't, return nothing
        private_intersect(r, this->head.get(),current_level_outputs); // Recursively get all the intersections inside the world, then return
        std::sort(current_level_outputs.begin(), current_level_outputs.end(), [](const Impact &a, const Impact &b){return a.get_t() < b.get_t();}); // sort by impact time
        return current_level_outputs;
    }
    return {};
}

unsigned long BVH::private_count_nodes(const AABB* cur_node) const{
    unsigned long count = 1;
    auto left =   cur_node->get_left();
    auto right =   cur_node->get_right();
    if(left){count += private_count_nodes(left);}
    if(right){count += private_count_nodes(right);}
    for(auto const& child: cur_node->get_center()){
        if(child){
            count += private_count_nodes(child);
        }
    }
    return count;
}

unsigned long BVH::count_nodes() const{
    if(this->head){
        return private_count_nodes(this->head.get());
    }
    return 0;
}

void BVH::print(bool verbose) const{
    std::cout << "Head" << std::endl;
    head->print(verbose);    
}