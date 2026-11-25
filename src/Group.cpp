#include "Group.h"
#include "Impact.h"
#include <algorithm>

Group::Group(const Matrix& Transformation, const Material& material, Shape* parent): Shape(Transformation,material,parent,"Group"){}

void Group::verbose_print() const{
    std::cout << this->get_name() << std::endl;
    std::cout << this->get_id() << std::endl;
    std::cout << this->get_transform() << std::endl;
    std::cout << this->get_material() << std::endl;
}

// Groups are flattened into World's BVH, so this is only hit when a Group is intersected directly.
// Shape::intersect expects a world-space ray (it applies the aggregate transform), but `r` here is
// already in this group's local space, so convert it back before handing it to the children.
std::vector<Impact> Group::local_intersect(const Ray &r) const {
    if (children.empty()) return {};
    Ray world_ray = r.transform(get_aggregate_transform());
    std::vector<Impact> out;
    for(auto const& child: children){
        for(auto hit: child->intersect(world_ray)){
            out.push_back(hit);
        }
    }
    return out;
}

Tuple Group::local_normal_at(const Tuple& pt, const Impact& impt) const{
    throw std::invalid_argument("Group has no local normal");
}

// A single box (in group space) tagged with this group -- not a tree. If this returned a tree of
// child boxes, a parent BVH would hand out the children as candidates, and their boxes would be
// in the wrong space since AABB::transform only moves the root box.
AABB Group::bound() const{
    AABB output;
    for(auto const& child: children){
        AABB bbox = parent_space_bounds(child.get());
        if(!bbox.is_finite()){
            return AABB({NEG_INFTY,NEG_INFTY,NEG_INFTY},{INFTY,INFTY,INFTY},this);
        }
        output.expand_box(&bbox);
    }
    output.set_shape(this);
    return output;
}
