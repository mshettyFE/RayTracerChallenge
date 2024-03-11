#include "Group.h"

Group::Group(Matrix Transformation, Material material, std::shared_ptr<Shape> parent){
    this->mat = material;
    this->Transformation = Transformation;
    this->parent = parent;
//    this->this_group = std::make_shared<Shape>(*this);
}

Tuple Group::normal_at(const Tuple& pt) const{}
void Group::print() const {}
std::vector<double> Group::intersect(const Ray &other) const {}
int Group::get_total_members() const{
    return this->members.size();
}
void Group::add_child(std::shared_ptr<Shape> new_member){
//    new_member->set_parent(this_group);
    this->members.push_back(new_member);
}
