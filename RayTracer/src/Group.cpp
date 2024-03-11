#include "Group.h"

Group::Group(Matrix Transformation, Material material, std::shared_ptr<Shape> parent): Shape(Transformation,material,parent,"Group"){
    this->this_group = std::make_shared<Group>(*this);
}

std::shared_ptr<Shape> Group::get_group_pointer() const{
    return this->this_group;
}

void Group::indent_print(unsigned int tabs) const{
    auto indentation = std::string(tabs,'\t');
    std::cout << indentation <<"Shape: " <<  name << " ID:" << get_id() << " Total Children: " << get_total_children() << std::endl;
    tabs++;
    for(auto child: children){
        if(child->get_total_children() != 0){
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

Tuple Group::normal_at(const Tuple& pt) const{}
std::vector<double> Group::intersect(const Ray &other) const {}