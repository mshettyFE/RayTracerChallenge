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
    std::vector<std::shared_ptr<Shape>> members;
//    std::shared_ptr<Shape> this_group;
public:
    Tuple normal_at(const Tuple& pt) const override;
    void print() const override;
    std::vector<double> intersect(const Ray &other) const override;

    Group(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

    int get_total_members() const;

    void add_child(std::shared_ptr<Shape> new_member);
};

#endif