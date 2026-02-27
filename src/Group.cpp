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

void Group::build_bvh() const {
    if (bvh_built) return;
    if (children.empty()) {
        bvh_built = true;
        return;
    }
    bvh_cache = bound();
    bvh_built = true;
}

std::vector<Impact> Group::local_intersect(const Ray &r) const {
    if (children.empty()) return {};

    if (!bvh_built) build_bvh();

    if (bvh_cache) {
        if (!bvh_cache->intersect(r)) return {};

        std::vector<const Shape*> candidates;
        bvh_cache->collect_candidates(r, candidates);

        std::vector<Impact> out;
        for (auto const* shape : candidates) {
            for (auto hit : shape->intersect(r)) {
                out.push_back(hit);
            }
        }
        return out;
    }

    // Fallback: brute-force
    std::vector<Impact> out;
    for(auto const& child: children){
        for(auto hit: child->intersect(r)){
            out.push_back(hit);
        }
    }
    return out;
}

Tuple Group::local_normal_at(const Tuple& pt, const Impact& impt) const{
    throw std::invalid_argument("Group has no local normal");
}

std::unique_ptr<AABB> Group::bound() const{
    AABB output;
    std::vector<std::unique_ptr<AABB>> boxes;
    for(auto const& child: children){
        auto bbox = parent_space_bounds(child.get());
        output.expand_box(bbox.get());
        boxes.push_back(std::move(bbox));
    }
    for(unsigned int i=0; i<boxes.size(); ++i){
        output.insert(boxes[i]);
    }
    return std::make_unique<AABB>(std::move(output));
}
