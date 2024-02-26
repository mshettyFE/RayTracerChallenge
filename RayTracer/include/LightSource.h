#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Color.h"
#include "Tuple.h"
#include "Shape.h"

class LightSource{
public:
    virtual void print() const=0;
    virtual Color shade(const Material& mat, const Tuple& position,
        const Tuple& camera, const Tuple& normal) const=0;

};

#endif