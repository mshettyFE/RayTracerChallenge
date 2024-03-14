#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Color.h"
#include "Tuple.h"

class Shape;

class LightSource{
    protected:
        Color Intensity;
        Tuple SourceLoc;
        std::string name;
    public:
        LightSource(const Color& intensity,const Tuple& position, const std::string& light_type="PLACEHOLDER");
        LightSource(const LightSource&) = delete;
        LightSource& operator= (const LightSource&) = delete;
        LightSource(LightSource&& mS)            = default;
        LightSource& operator=(LightSource&& mS) = default;
        virtual ~LightSource() {}

        Color get_intensity() const;
        Tuple get_position() const;

        std::string get_name() const;

        virtual void print() const=0;
        // This assumes that you want to apply the pattern to conform to some object
        virtual Color shade(const Shape* obj, const Tuple& position,
        const Tuple& camera, const Tuple& normal, bool in_shadow=false) const =0;
};

#endif