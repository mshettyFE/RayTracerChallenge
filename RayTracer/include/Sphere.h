#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Matrix.h"
#include "Tuple.h"

class Sphere: public Shape{
    Tuple normal_at(Tuple pt);
    void print();
};

#endif