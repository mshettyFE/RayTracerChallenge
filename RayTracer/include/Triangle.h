#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "Tuple.h"
#include "Ray.h"
#include <initializer_list>

class Triangle : public Shape{
private:
    Tuple p1;
    Tuple p2;
    Tuple p3;
    Tuple edge1;
    Tuple edge2;
    Tuple normal;
    Tuple local_normal_at(const Tuple& pt) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
public:
    Triangle(const std::initializer_list<double> p1, const std::initializer_list<double> p2,const std::initializer_list<double> p3, const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), Shape* parent=nullptr);
    Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), Shape* parent=nullptr);

    void verbose_print() const override;

    std::vector<Tuple> get_points() const;
    Tuple get_edge1() const;
    Tuple get_edge2() const;

};

#endif
