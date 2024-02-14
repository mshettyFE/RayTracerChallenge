#ifndef RAY_H
#define RAY_H

#include "Tuple.h"
#include "Matrix.h"

class Ray{
    protected:
        Tuple origin; // Must be a Point
        Tuple direction; // Must be a Direction
    public:
        Ray(Tuple a_origin, Tuple a_direction);
        Ray(std::initializer_list<double> a_origin, std::initializer_list<double> a_direction);

        bool operator==(const Ray& other) const;
        bool operator!=(const Ray& other) const;

        Ray transform(const Matrix& other) const;

        // For a given time, return the point along the ray
        Tuple position(double t);

        friend std::ostream& operator << (std::ostream &out, const Ray& other);

};



#endif