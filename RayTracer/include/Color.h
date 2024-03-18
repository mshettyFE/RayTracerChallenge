#ifndef COLOR_H
#define COLOR_H

#include "Tuple.h"
#include "Constants.h"
#include "RNG.h"
#include <random>

#define RED Color(1,0,0)
#define GREEN Color(0,1,0)
#define BLUE Color(0,0,1)
#define YELLOW Color(0xFFFF00)
#define PURPLE Color(0xFF00FF)
#define BLACK Color(0,0,0)
#define WHITE Color(1,1,1)
#define GRAY Color(0.5,0.5,0.5)
#define RED_CHANNEL(hex) ((hex >> 16)&0x0000FF)
#define GREEN_CHANNEL(hex) ((hex >> 8)&0x0000FF)
#define BLUE_CHANNEL(hex) hex & 0x0000FF

class Color : public Tuple{

    public:
    // Constructors
    // Internally, used Vectors instead of Points
    // assumes max value of r,g,b is 1. Doesn't enforce this constraint since some transformations might more you out of this range
        Color(double red, double green, double blue);
        Color(int hex);
        Color();

        // Convinience functions to extract RGB channel values
        const double get_red() const;
        const double get_green() const ;
        const double get_blue() const;

        // Effectively a Hadamard product between color channels
        Color operator*(const Color& other) const;
        Color operator+(const Color& other) const;
        Color operator-(const Color& other) const;

        template <  typename T>
        Color operator*(T scalar) const{
        static_assert(std::is_arithmetic<T>::value,"Need to multiply color by a number");
            double r = this->get_red()*scalar;
            double g = this->get_green()*scalar;
            double b = this->get_blue()*scalar;
            return Color(r,g,b);
        }


        // force rgb values to be between 0 or 1
        void clamp();
        // clamp all channels, then scale to (0,255)
        void rescale();
        // normalize Color vector
        void normalize();

        friend std::ostream& operator << (std::ostream &out, const Color& other);

};

template <typename T>
Color operator*(T scalar, Color const & other) {
    static_assert(std::is_arithmetic<T>::value,"Need to multiply color by a number");
    return other * scalar;
}

Color random_color();

#endif