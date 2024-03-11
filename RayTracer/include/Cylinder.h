#ifndef CYLINDER_H
#define CYLINDER_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include "Impact.h"
#include <limits>

class Cylinder : public Shape{
private:
    double minimum;
    double maximum;
    bool closed;
    bool check_cap(const Ray& r, double time) const;
    std::vector<Impact> intersect_cap(const Ray& r) const;
    Tuple local_normal_at(const Tuple& pt) const override;
public:
    void verbose_print() const override;
    std::vector<Impact> intersect(const Ray &other) const override;

    Cylinder(Matrix Transformation=MatIdentity(4), Material material=Material(), std::shared_ptr<Shape> parent=nullptr,
     double min= NEG_INFTY, double max= INFTY,bool closed = false);

    void set_min(double val) ;
    void set_max(double val) ;
    void set_closed(bool val) ;

    double get_min() const;
    double get_max() const;
    bool get_closed() const;
};

#endif