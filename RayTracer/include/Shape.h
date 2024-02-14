#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"

class Shape{
private:
    static unsigned long GlobalID;
protected:
    unsigned long id;
    Matrix Transformation;
public:
    virtual Tuple normal_at(Tuple pt)=0;
    virtual void print() const=0;

    Shape();
    Shape(Matrix Transformation);
    void set_transform(Matrix Tranformation);
};

#endif