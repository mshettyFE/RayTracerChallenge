#ifndef COLOR_H
#define COLOR_H

#include "Tuple.h"

class Color : public Tuple{

    public:
    // Constructors
    // Internally, used Vectors instead of Points
        Color(double red, double green, double blue, double resolution=1E-9);

        const double get_red();
        const double get_green();
        const double get_blue();

        Color operator*(Color& other);
};

#endif