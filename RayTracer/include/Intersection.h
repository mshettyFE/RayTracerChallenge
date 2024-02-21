#ifndef INTERSECTION_INFO_H
#define INTERSECTION_INFO_H

#include "Shape.h"
#include "Tuple.h"
#include <memory>

class Intersection{
private:
    double time;
    std::shared_ptr<Shape> obj;
    Tuple pnt;
    Tuple eye;
    Tuple normal;
    bool inside;
public:
    Intersection(double time,const std::shared_ptr<Shape>& obj, const Ray& ray);
    
    double get_t() const;
    std::shared_ptr<Shape> get_obj() const;
    Tuple get_pnt() const;
    Tuple get_eye() const;
    Tuple get_normal() const;
    bool is_inside() const;

};

#endif