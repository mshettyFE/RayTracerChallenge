#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include <memory>

class Shape{
private:
    static inline unsigned long GlobalID{0};
protected:
    unsigned long id;
    Matrix Transformation;
    Material mat;
    std::shared_ptr<Shape> parent;
public:
    virtual Tuple normal_at(const Tuple& pt) const =0;
    virtual void print() const=0;
    virtual std::vector<double> intersect(const Ray &other) const =0;

    Shape(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

    Matrix get_transform() const;
    void set_transform(Matrix Tranformation);

    void set_material(Material new_mat);
    Material get_material() const;
    unsigned long get_id() const;

    std::shared_ptr<Shape> get_parent() const;
    void set_parent(std::shared_ptr<Shape> new_parent);

};

#endif