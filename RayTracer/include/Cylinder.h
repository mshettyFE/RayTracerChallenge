#ifndef CYLINDER_H
#define CYLINDER_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include <limits>

class Cylinder : public Shape{
private:
    double minimum;
    double maximum;
    bool closed;
    bool check_cap(const Ray& r, double time) const;
    std::vector<double> intersect_cap(const Ray& r) const;
public:
    Tuple normal_at(const Tuple& pt) const override;
    void print() const override;
    std::vector<double> intersect(const Ray &other) const override;

    Cylinder(Matrix Transformation=MatIdentity(4), Material material=Material(),
     double min= NEG_INFTY, double max= INFTY,bool closed = false);

    void set_min(double val) ;
    void set_max(double val) ;
    void set_closed(bool val) ;

    double get_min() const;
    double get_max() const;
    bool get_closed() const;
};

#endif