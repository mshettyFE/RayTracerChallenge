#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Color.h"
#include "Tuple.h"
#include "Shape.h"

class LightSource{
    protected:
        Color Intensity;
        Tuple SourceLoc;
    public:
        LightSource(const Color& intensity,const Tuple& position);

        Color get_intensity() const;
        Tuple get_position() const;

        virtual void print() const=0;
        virtual Color shade(const Material& mat, const Tuple& position,
            const Tuple& camera, const Tuple& normal, bool in_shadow=false) const=0;
};

#endif