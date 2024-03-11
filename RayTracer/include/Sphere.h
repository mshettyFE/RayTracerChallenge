#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Ray.h"
#include "Material.h"
#include "Impact.h"
#include <vector>

class Sphere: public Shape{
    private:
        Tuple local_normal_at(const Tuple& pt) const override;
    public:
        Sphere(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

        std::vector<Impact> intersect(const Ray &other) const override;
        void verbose_print() const override;
};

Sphere glass_sphere(double IOR=GLASS);

#endif