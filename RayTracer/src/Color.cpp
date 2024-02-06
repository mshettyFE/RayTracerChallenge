#include "Color.h"
#include <stdexcept>

Color::Color(double red, double green, double blue, double a_resolution){
    data.clear();
    data.push_back(red);
    data.push_back(green);
    data.push_back(blue);
    data.push_back(TupType::VECTOR);
    dim = 3;
    resolution = a_resolution;
}

const double Color::get_red(){return data[0];}
const double Color::get_green(){return data[1];}
const double Color::get_blue(){return data[2];}

Color Color::operator*(Color& other){
    if((this->dim != other.dim) && (other.dim !=4)){
        throw std::invalid_argument("Dimensions of color don't match");
    }
    double r = this->data[0]*other.data[0];
    double g = this->data[1]*other.data[1];
    double b = this->data[2]*other.data[2];
    return Color(r,g,b);
}