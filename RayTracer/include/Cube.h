#ifndef CUBE_H
#define CUBE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Impact.h"

class Cube : public Shape{
private:
    std::vector<double> check_axis(double origin, double direction) const;
    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
public:
    void verbose_print() const override;

    Cube(Matrix Transformation=MatIdentity(4), Material material=Material(), Shape* parent=nullptr);
};

#endif