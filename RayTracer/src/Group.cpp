#include "Group.h"

Group::Group(Matrix Transformation, Material material, std::shared_ptr<Shape> parent){
    this->mat = material;
    this->Transformation = Transformation;
    this->parent = parent;
    this->this_group = std::make_shared<Group>(*this);
}

std::shared_ptr<Shape> Group::get_group_pointer() const{
    return this->this_group;
}

void Group::print(unsigned int indent) const{
    auto tabs = std::string(indent,'\t');
    std::cout << tabs << "Group" << std::endl;
    indent++;
    tabs = std::string(indent,'\t');
    for(auto child: children){
        std::cout << tabs << child->get_id() << std::endl;
        if(child->get_total_children() != 0){
            child->print(indent);
        }
    }
}

Tuple Group::normal_at(const Tuple& pt) const{}
std::vector<double> Group::intersect(const Ray &other) const {}