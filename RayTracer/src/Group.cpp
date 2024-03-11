#include "Group.h"
#include "Impact.h"
#include <algorithm>

Group::Group(Matrix Transformation, Material material, std::shared_ptr<Shape> parent): Shape(Transformation,material,parent,"Group"){
    this->nested_shape_type = NestedShapeType::GROUP;
}

int Group::get_total_children() const{
    return this->children.size();
}

void Group::indent_print(unsigned int tabs) const{
    auto indentation = std::string(tabs,'\t');
    this->print(tabs);
    tabs++;
    for(auto child: children){
        if(child->get_nst() == NestedShapeType::GROUP){
            std::dynamic_pointer_cast<Group>(child)->indent_print(tabs);
        }
        else{
            child->print(tabs);
        }
    }
}

void Group::verbose_print() const{
    indent_print(0);
}

std::vector<Impact> Group::intersect(const Ray &r) const {
    std::vector<Impact> out;
    for(auto child: children){
        for(auto hit: child->intersect(r)){
            out.push_back(hit);
        }
    }
//    std::sort(out.begin(), out.end(), [](double a, double b) -> bool{ return (a < b);  });
    return out;
}

Tuple Group::local_normal_at(const Tuple& pt) const{
    throw std::invalid_argument("Group has no local normal");
}
