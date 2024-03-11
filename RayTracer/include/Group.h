#ifndef GROUP_H
#define GROUP_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Group.h"
#include "Impact.h"
#include <memory>
#include <map>
#include <type_traits>

class Group : public Shape{
private:
    void indent_print(unsigned int tabs=0) const;
    std::vector<std::shared_ptr<Shape>> children;
    Tuple local_normal_at(const Tuple& pt) const override;
public:
    void verbose_print() const override;
    std::vector<Impact> intersect(const Ray &other) const override;

    int get_total_children() const;

    Group(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

template<class shp>
void add_child(shp& new_member){
        static_assert(std::is_base_of<Shape,shp>::value, "shp must be derived from Shape");
        new_member.set_parent(std::make_shared<Group>(*this));
        this->children.push_back(std::make_shared<shp>(new_member));
    }
};

#endif