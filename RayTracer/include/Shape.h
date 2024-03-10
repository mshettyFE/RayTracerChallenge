#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"

class Shape{
private:
    static inline unsigned long GlobalID{0};
protected:
    unsigned long id;
    Matrix Transformation;
    Material mat;
public:
    virtual Tuple normal_at(Tuple pt) const =0;
    virtual void print() const=0;
    virtual std::vector<double> intersect(const Ray &other) const =0;

    Shape(Matrix Transformation=MatIdentity(4), Material material=Material());

    Matrix get_transform() const;
    void set_transform(Matrix Tranformation);

    Material get_material() const;
    unsigned long get_id() const;

};

#endif