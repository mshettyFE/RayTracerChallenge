#ifndef SMOOTH_TRIANGLE_H
#define SMOOTH_TRIANGLE_H

#include "Shape.h"
#include "Tuple.h"
#include "Ray.h"
#include <initializer_list>

class SmoothTriangle : public Shape{
private:
    Tuple p1;
    Tuple p2;
    Tuple p3;
    Tuple n1;
    Tuple n2;
    Tuple n3;
    Tuple edge1;
    Tuple edge2;
    Tuple local_normal_at(const Tuple& pt,const Impact& impt=Impact()) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
public:
    SmoothTriangle(const std::initializer_list<double> p1, const std::initializer_list<double> p2,const std::initializer_list<double> p3,const std::initializer_list<double> n1, const std::initializer_list<double> n2,const std::initializer_list<double> n3, const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), Shape* parent=nullptr);
    SmoothTriangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Tuple& n1, const Tuple& n2, const Tuple& n3, const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), Shape* parent=nullptr);

    void verbose_print() const override;

    std::vector<Tuple> get_points() const;
    std::vector<Tuple> get_normals() const;

    std::unique_ptr<AABB> bound() const override;

};

#endif