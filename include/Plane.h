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
        Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override;
        std::vector<Impact> local_intersect(const Ray &other) const override;
    public:
        Plane(Matrix Transformation=MatIdentity(4), Material material=Material(), Shape* parent=nullptr);

        void verbose_print() const override;
        std::unique_ptr<AABB> bound() const override;



};
#endif