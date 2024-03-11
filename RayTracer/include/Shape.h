#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Impact.h"
#include <memory>

class Shape{
private:
    static inline unsigned long GlobalID{0};
    Matrix Transformation;
    virtual Tuple local_normal_at(const Tuple& pt) const =0;
protected:
    unsigned long id;
    Material mat;
    std::shared_ptr<Shape> parent;
    std::string name;
    NestedShapeType nested_shape_type = NestedShapeType::UNNESTED;
    Matrix get_only_this_transform() const;
public:
    Tuple normal_at(const Tuple& pt) const;
    virtual std::vector<Impact> intersect(const Ray &other) const =0;
    virtual ~Shape(){}

    virtual void verbose_print() const=0;
    void print(unsigned int indent=0) const;

    Shape(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr, std::string="PLACEHOLDER");

    Matrix get_transform(int count = 0, bool verbose=false) const;
    void set_transform(Matrix Tranformation);

    std::string get_name() const;

    void set_material(Material new_mat);
    Material get_material() const;
    unsigned long get_id() const;

    void set_nst(NestedShapeType new_nst);
    NestedShapeType get_nst() const;

    std::shared_ptr<Shape> get_parent() const;
    void set_parent(std::shared_ptr<Shape> new_parent);

};

#endif