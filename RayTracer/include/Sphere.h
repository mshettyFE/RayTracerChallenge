#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Ray.h"
#include <vector>

class Sphere: public Shape{
    public:
        Sphere();
        Sphere(Matrix Transformation);

        Tuple normal_at(Tuple pt);
        std::vector<double> intersect(const Ray &other) const;

        void print() const;
        friend std::ostream& operator << (std::ostream &out, const Sphere& other);
};

#endif