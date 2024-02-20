#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Ray.h"
#include "Material.h"
#include <vector>

class Sphere: public Shape{
    public:
        Sphere(Matrix Transformation=MatIdentity(4), Material material=Material());

        Tuple normal_at(Tuple pt) const override;
        std::vector<double> intersect(const Ray &other) const override;
        void print() const override;

        friend std::ostream& operator << (std::ostream &out, const Sphere& other);
};

#endif