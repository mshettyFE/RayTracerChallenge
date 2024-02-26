#ifndef POINT_SOURCE_H
#define POINT_SOURCE_H

#include "Color.h"
#include "Tuple.h"
#include "Shape.h"
#include "LightSource.h"

class PointSource: public LightSource{
private:
    Color Intensity;
    Tuple SourceLoc;
public:
    PointSource(Color a_intensity, Tuple a_position);
    Color get_intensity() const;
    Tuple get_position() const;

    // position is where normal and color is to be shaded
    Color shade(const Material& mat, const Tuple& position,
        const Tuple& camera, const Tuple& normal) const override;

    friend std::ostream& operator << (std::ostream &out, const PointSource& other);
};

#endif