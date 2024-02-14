#include "Color.h"
#include <stdexcept>
#include <cmath>

Color::Color(){
    data.clear();
    data.push_back(0);
    data.push_back(0);
    data.push_back(0);
    data.push_back(TupType::VECTOR);
    dim = 3;
    resolution = glob_resolution;
}

Color::Color(double red, double green, double blue, double a_resolution){
    data.clear();
    data.push_back(red);
    data.push_back(green);
    data.push_back(blue);
    data.push_back(TupType::VECTOR);
    dim = 3;
    resolution = a_resolution;
}

Color::Color(int hex, double a_resolution){
    data.clear();
    double red = RED_CHANNEL(hex);
    double green = GREEN_CHANNEL(hex);
    double blue = BLUE_CHANNEL(hex);
    data.push_back(red/255.0);
    data.push_back(green/255.0);
    data.push_back(blue/255.0);
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

void Color::clamp(){
    double temp = 0;
    for(int i=0; i< dim; ++i){
        if(data[i]<0){
            data[i] = 0.0;
        }
        if(data[i] > 1){
            data[i] = 1.0;
        }
    }
}

void Color::rescale(){
    this->clamp();
    for(int i=0; i< dim; ++i){
        data[i] *= 255;
    }
}

void Color::normalize(){
    for(int i=0; i<dim; ++i){
        data[i] /= 255;
    }
}