#ifndef POINT_SOURCE_H
#define POINT_SOURCE_H

#include "LightSource.h"
#include <memory>

class Shape;

class PointSource: public LightSource{
public:
    PointSource(const Color& a_intensity,const Tuple& a_position);
    Color shade(const Shape* obj, const Tuple& position,
        const Tuple& camera, const Tuple& normal, bool in_shadow=false) const override;
    
    void print() const override;


    friend std::ostream& operator << (std::ostream &out, const PointSource& other);
};

#endif