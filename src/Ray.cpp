#include "Ray.h"
#include <stdexcept>

Ray::Ray(const Tuple& a_origin, const Tuple& a_direction) : origin(a_origin), direction(a_direction){
    if(origin.type() != TupType::POINT){
        throw std::invalid_argument("Ray Origin is not a point");
    }
    if(direction.type() != TupType::VECTOR){
        throw std::invalid_argument("Ray Direction is not a vector");
    }
}

// The Tuple constructor rejects lists that aren't exactly 3 coordinates.
Ray::Ray(std::initializer_list<double> a_origin, std::initializer_list<double> a_direction)
    : origin(a_origin, TupType::POINT), direction(a_direction, TupType::VECTOR){}

bool Ray::operator==(const Ray& other) const{
    return (this->origin ==other.origin) && (this->direction == other.direction);
}
bool Ray::operator!=(const Ray& other) const{
    bool v = (this->origin ==other.origin) && (this->direction == other.direction);
    return !v;
}



Tuple Ray::position(double t) const{
    return origin+direction*t;
}

Ray Ray::transform(const Matrix& other) const{
    return Ray(other*origin, other*direction);
}

std::ostream& operator << (std::ostream &out, const Ray& other){
    out << "Ray:\n";
    out << "\tOrigin: " << other.origin;
    out << "\tDirection: " << other.direction << std::endl;
    return out;
}
