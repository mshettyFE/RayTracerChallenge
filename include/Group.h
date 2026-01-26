#ifndef GROUP_H
#define GROUP_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Impact.h"
#include <memory>
#include <map>
#include <type_traits>

class Group : public Shape{
private:
    void indent_print(unsigned int tabs=0) const;
    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
public:
    void verbose_print() const override;
    std::unique_ptr<AABB> bound() const override;

    Group(const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), Shape* parent=nullptr);

    virtual const Shape* add_child(std::unique_ptr<Shape> new_member){
        unsigned long new_id = new_member->get_id();
        new_member->set_parent(this);
        this->children.push_back(std::move(new_member));
        return this->children[get_total_children()-1].get();
    }
};

#endif