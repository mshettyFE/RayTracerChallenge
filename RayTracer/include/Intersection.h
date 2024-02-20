#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Shape.h"
#include <memory>

struct Intersection{
    double t;
    std::shared_ptr<Shape> shape_type;
};

#endif