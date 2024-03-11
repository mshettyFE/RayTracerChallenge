#ifndef CUBE_H
#define CUBE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"

class Cube : public Shape{
private:
    std::vector<double> check_axis(double origin, double direction) const;
public:
    Tuple normal_at(const Tuple& pt) const override;
    void verbose_print() const override;
    std::vector<double> intersect(const Ray &other) const override;

    Cube(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);
};

#endif