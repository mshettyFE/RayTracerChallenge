#ifndef POINT_SOURCE_H
#define POINT_SOURCE_H

#include "Color.h"
#include "Tuple.h"
#include "Shape.h"
#include "LightSource.h"
#include <memory>

class PointSource: public LightSource{
public:
    PointSource(const Color& a_intensity,const Tuple& a_position);
    Color shade(const std::shared_ptr<Shape> obj, const Tuple& position,
        const Tuple& camera, const Tuple& normal, bool in_shadow=false) const override;
    
    void print() const override;


    friend std::ostream& operator << (std::ostream &out, const PointSource& other);
};

#endif