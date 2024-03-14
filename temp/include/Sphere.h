#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere: public Shape{
    private:
        Tuple local_normal_at(const Tuple& pt) const override;
        std::vector<Impact> local_intersect(const Ray &other) const override;
    public:
        Sphere(const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), const Shape* parent=nullptr);

        void verbose_print() const override;
};

Sphere glass_sphere(double IOR=GLASS);

#endif