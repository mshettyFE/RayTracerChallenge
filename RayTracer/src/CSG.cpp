#include <algorithm>
#include "CSG.h"

CSG::CSG(CSG_OPS join_type, std::unique_ptr<Shape> left, std::unique_ptr<Shape> right, Matrix Transformation) : Shape(Transformation, Material(), nullptr, "CSG"){
    this->join_type  = join_type;
    left->set_parent(this);
    right->set_parent(this);
    this->children.push_back(std::move(left));
    this->children.push_back(std::move(right));
}

bool CSG::intersection_allowed(bool lhit, bool inl, bool inr) const{
    switch(join_type){
        case UNION:
            return (lhit && !(inr)) || (!(lhit) && !(inl));
            break;
        case INTERSECTION:
            return (lhit && inr) || (!(lhit) && inl);
            break;
        case DIFFERENCE:
            return (lhit && !(inr)) || (!(lhit) && inl);
            break;
        default:
            break;
    }
    throw std::invalid_argument("Unimplemented OP type");
    return false;
}

std::vector<Impact> CSG::filter_impacts(const std::vector<Impact>& hits) const{
    bool inl = false;
    bool inr = false;
    std::vector<Impact> output;
    for(auto const& hit: hits){
        bool lhit  = get_left()->includes(hit.get_obj());
        if(intersection_allowed(lhit,inl,inr)){
            output.push_back(hit);
        }
        if(lhit){
            inl = !inl;
        }
        else{
            inr = !inr;
        }
    }
    return output;
}

std::vector<Impact> CSG::local_intersect(const Ray &other) const {
    auto left_hits = get_left()->intersect(other);
    auto right_hits = get_right()->intersect(other);
    std::sort(left_hits.begin(), left_hits.end());
    std::sort(right_hits.begin(), right_hits.end());
    std::vector<Impact> all_hits(left_hits.size()+right_hits.size());
    std::merge(left_hits.begin(), left_hits.end(), right_hits.begin(), right_hits.end(), all_hits.begin());
    return filter_impacts(all_hits);
}

Tuple CSG::local_normal_at(const Tuple& pt, const Impact& impt) const{std::invalid_argument("CSG doesn't have a normal (like group)"); return GenVec(0,0,0);}
void CSG::verbose_print() const {}

std::unique_ptr<AABB> CSG::bound() const{
    AABB output;
    for(auto const& child: children){
        auto bbox = parent_space_bounds(child.get());
        output.expand_box(*bbox.get());
    }
    return std::make_unique<AABB>(output);
}
 