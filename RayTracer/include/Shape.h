#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"

class Shape{
private:
    static unsigned long GlobalID;
protected:
    unsigned long id;
    Matrix Transformation;
    Material mat;
public:
    virtual Tuple normal_at(Tuple pt) const =0;
    virtual void print() const=0;

    Shape(Matrix Transformation=MatIdentity(4), Material material=Material());
    void set_transform(Matrix Tranformation);

    Material get_material() const;

};

#endif