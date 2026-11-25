#ifndef RAY_H
#define RAY_H

#include "Tuple.h"
#include "Matrix.h"

class Ray{
    protected:
        Tuple origin; // Must be a Point
        Tuple direction; // Must be a Direction
    public:
        // a_origin must be a point and a_direction a vector; throws otherwise.
        Ray(const Tuple& a_origin, const Tuple& a_direction);
        // Each list must be exactly 3 coordinates; throws otherwise.
        Ray(std::initializer_list<double> a_origin, std::initializer_list<double> a_direction);

        const Tuple& get_origin() const{return origin;}
        const Tuple& get_direction() const{return direction;}


        bool operator==(const Ray& other) const;
        bool operator!=(const Ray& other) const;

        Ray transform(const Matrix& other) const;

        // For a given time, return the point along the ray
        Tuple position(double t) const;

        friend std::ostream& operator << (std::ostream &out, const Ray& other);

};



#endif