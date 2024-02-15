#ifndef POINT_SOURCE_H
#define POINT_SOURCE_H

#include "Color.h"
#include "Tuple.h"
#include "Shape.h"

class PointSource{
private:
    Color Intensity;
    Tuple SourceLoc;
public:
    PointSource(Color a_intensity, Tuple a_position);
    Color get_intensity() const;
    Tuple get_position() const;

    Color shade(const Material& mat, const Tuple& position,
        const Tuple& camera, const Tuple& normal) const;

    friend std::ostream& operator << (std::ostream &out, const PointSource& other);
};

#endif