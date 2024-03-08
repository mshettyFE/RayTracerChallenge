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
        // position is where normal and color is to be shaded
        // This assumes that you want to apply a pattern to this point independent of the object
        virtual Color shade(const Material& mat, const Tuple& position,
            const Tuple& camera, const Tuple& normal, bool in_shadow=false) const=0;
        // This assumes that you want to apply the pattern to conform to some object
        virtual Color shade(const std::shared_ptr<Shape> obj, const Tuple& position,
        const Tuple& camera, const Tuple& normal, bool in_shadow=false) const =0;
};

#endif