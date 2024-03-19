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

Tuple CSG::local_normal_at(const Tuple& pt, const Impact& impt) const{std::invalid_argument("CSG doesn't have a normal (like group)"); return GenVec(0,0,0);}
std::vector<Impact> CSG::local_intersect(const Ray &other) const {return {};}
void CSG::verbose_print() const {}
std::unique_ptr<AABB> CSG::bound() const {return nullptr;}
 