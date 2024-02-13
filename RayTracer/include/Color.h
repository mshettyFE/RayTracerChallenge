#ifndef COLOR_H
#define COLOR_H

#include "Tuple.h"
#include "Constants.h"

#define RED Color(1,0,0)
#define GREEN Color(0,1,0)
#define BLUE Color(0,0,1)
#define RED_CHANNEL(hex) ((hex >> 16)&0x0000FF)
#define GREEN_CHANNEL(hex) ((hex >> 8)&0x0000FF)
#define BLUE_CHANNEL(hex) hex & 0x0000FF

class Color : public Tuple{

    public:
    // Constructors
    // Internally, used Vectors instead of Points
    // assumes max value of r,g,b is 1. Doesn't enforce this constraint since some transformations might more you out of this range
        Color(double red, double green, double blue, double a_resolution=glob_resolution);
        Color(int hex, double a_resolution=glob_resolution);
        Color();

        const double get_red();
        const double get_green();
        const double get_blue();

        Color operator*(Color& other);

        // force rgb values to be between 0 or 1
        void clamp();
        // clamp all channels, then scale to (0,255)
        void rescale();
        // normalize Color vector
        void normalize();
};

#endif