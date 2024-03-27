#include "BVH.h"
#include "Shape.h"

BVH::BVH(const std::vector<std::unique_ptr<Shape>>& shapes){
// define empty head
    this->head = std::make_unique<AABB>(std::move(AABB()));
    std::vector<std::unique_ptr<AABB>> boxes;
    for(auto const& shape: shapes){
        auto current_bb = shape->bound();
        head->expand_box(current_bb.get());
        boxes.push_back(std::move(current_bb));
    }
    for(int i=0; i<boxes.size(); ++i){
        head->insert(boxes[i]);
    }
}

void BVH::private_intersect(const Ray& r, const AABB* current_node, std::vector<Impact>& out_vector  ,unsigned long depth) const{
    // If the current node is a leaf, then return the intersections of the associated shape
    if(current_node->is_leaf()){
        auto hits = current_node->get_shape()->intersect(r);
        std::cout << "Adding to Output: " << hits.size() << std::endl;
        out_vector.insert(out_vector.end(),hits.begin(), hits.end());
        return;
    }
    depth++;
    // need to do central children first since it is ambiguous which side it falls in
    for(auto const& center_child: current_node->get_center()){
        if(center_child->intersect(r)){
            std::cout << "Pre Center Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Pre Center Current Depth " <<  depth << std::endl;
            private_intersect(r,center_child,out_vector,  depth);
            std::cout << "Post Center Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Post Center Current Depth " <<  depth << std::endl;
        }
    }
    // otherwise, recurse on left children, right children, and central children, and join vectors, then return output
    auto lft = current_node->get_left();
    auto rght = current_node->get_right();
// early return if left intersects
    if(lft!= nullptr){
        if(lft->intersect(r)){
            std::cout << "Pre Center Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Pre Center Current Depth " <<  depth << std::endl;
            private_intersect(r,lft,out_vector, depth);
            std::cout << "Post Left Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Post Left Current Depth " <<  depth << std::endl;
        }
    }
    if(rght!= nullptr){
        if(rght->intersect(r)){
            std::cout << "Pre Right Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Pre Right Current Depth " <<  depth << std::endl;
            private_intersect(r,rght, out_vector, depth);
            std::cout << "Post Right Current Size: " <<  out_vector.size() << std::endl;
            std::cout <<"Post Right Current Depth " <<  depth << std::endl;
        }
    }
    std::cout << "Total Size: " << out_vector.size() << std::endl;
    return ;
}


std::vector<Impact> BVH::intersect(const Ray& r) const{
    std::vector<Impact> current_level_outputs;
    if(this->head->intersect(r)){ // check if the ray intersects the world. If it doesn't, return nothing
        private_intersect(r, this->head.get(),current_level_outputs); // Recursively get all the intersections inside the world, then return
        return current_level_outputs;
    }
    return {};
}

unsigned long BVH::private_count_nodes(const AABB* cur_node){
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


unsigned long BVH::count_nodes(){
    if(this->head){
        return private_count_nodes(this->head.get());
    }
    return 0;
}


void BVH::print() const{
    head->print();    
    std::cout << "Head Children: " <<  head->get_center().size() << std::endl;
}