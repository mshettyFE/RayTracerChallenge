#include "Ray.h"
#include <stdexcept>

Ray::Ray(Tuple a_origin, Tuple a_direction){
    if(a_origin.type() != TupType::POINT){
        throw std::invalid_argument("Ray Origin is not a point");
    }
    if(a_direction.type() != TupType::VECTOR){
        throw std::invalid_argument("Ray Direction is not a vector");
    }
    origin = a_origin;
    direction = a_direction;
}

Ray::Ray(std::initializer_list<double> a_origin, std::initializer_list<double> a_direction){
    if(a_origin.size() != 3){
        throw std::invalid_argument("Ray origin is not of length 3");
    }
    if(a_direction.size() != 3){
        throw std::invalid_argument("Ray direction is not of length 3");
    }
    origin = Tuple(a_origin, TupType::POINT);
    direction = Tuple(a_direction, TupType::VECTOR);
}

bool Ray::operator==(const Ray& other) const{
    return (this->origin ==other.origin) && (this->direction == other.direction);
}
bool Ray::operator!=(const Ray& other) const{
    bool v = (this->origin ==other.origin) && (this->direction == other.direction);
    return !v;
}

Tuple Ray::get_origin() const{return origin;};
Tuple Ray::get_direction() const{return direction;};


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
