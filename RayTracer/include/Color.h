#ifndef COLOR_H
#define COLOR_H

#include "Tuple.h"
#include "Constants.h"

#define RED Color(1,0,0)
#define BLUE Color(0,1,0)
#define GREEN Color(0,0,1)

class Color : public Tuple{

    public:
    // Constructors
    // Internally, used Vectors instead of Points
        Color(double red, double green, double blue, double a_resolution=glob_resolution);
        Color();

        const double get_red();
        const double get_green();
        const double get_blue();

        Color operator*(Color& other);
};

#endif