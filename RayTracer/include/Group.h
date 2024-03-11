#ifndef GROUP_H
#define GROUP_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Group.h"
#include <memory>
#include <map>

class Group : public Shape{
private:
    std::shared_ptr<Group> this_group;
    void indent_print(unsigned int tabs=0) const;
public:
    Tuple normal_at(const Tuple& pt) const override;
    void verbose_print() const override;
    std::vector<double> intersect(const Ray &other) const override;

    Group(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

    std::shared_ptr<Shape> get_group_pointer() const;

    template <typename shp>
    void add_child(shp& new_member){
        new_member.set_parent(this_group);
        this->children.push_back(std::make_shared<shp>(new_member));
    }
};

#endif