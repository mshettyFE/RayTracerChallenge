#ifndef RAY_H
#define RAY_H

#include "Tuple.h"

class Ray{
    protected:
        Tuple origin;
        Tuple direction;
    public:
        Ray(Tuple a_origin, Tuple a_direction);
        Ray(std::initializer_list<double> a_origin, std::initializer_list<double> a_direction);

        // overloaded binary operators
        bool operator==(const Ray& other) const;
        bool operator!=(const Ray& other) const;

        Tuple position(double t);

};



#endif