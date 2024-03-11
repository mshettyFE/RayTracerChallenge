#ifndef PLANE_H
#define PLANE_H
#include "Shape.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Ray.h"
#include "Impact.h"
#include <vector>

class Plane: public Shape{
    private:
        Tuple normal_vector;
        Tuple local_normal_at(const Tuple& pt) const override;
    public:
        Plane(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr);

        std::vector<Impact> intersect(const Ray &other) const override;
        void verbose_print() const override;


};
#endif