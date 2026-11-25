#include "Color.h"
#include <stdexcept>
#include <cmath>

Color::Color(){
    data = {0, 0, 0, TupType::VECTOR};
}

Color::Color(double red, double green, double blue){
    data = {red, green, blue, TupType::VECTOR};
}

Color::Color(int hex){
    double red = RED_CHANNEL(hex);
    double green = GREEN_CHANNEL(hex);
    double blue = BLUE_CHANNEL(hex);
    data = {red/255.0, green/255.0, blue/255.0, TupType::VECTOR};
}


const double Color::get_red() const {return data[0];}
const double Color::get_green() const {return data[1];}
const double Color::get_blue() const {return data[2];}

Color Color::operator+(const Color& other) const{
    return Color(this->get_red()+other.get_red(),this->get_green()+other.get_green(),this->get_blue()+other.get_blue());
}

Color Color::operator-(const Color& other) const{
    return Color(this->get_red()-other.get_red(),this->get_green()-other.get_green(),this->get_blue()-other.get_blue());
}

Color Color::operator*(const Color& other) const{
    double r = this->data[0]*other.data[0];
    double g = this->data[1]*other.data[1];
    double b = this->data[2]*other.data[2];
    return Color(r,g,b);
}

void Color::clamp(){
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

std::array<unsigned char,3> Color::to_rgb8() const{
    std::array<unsigned char,3> out;
    for(int i=0; i<3; ++i){
        double v = data[i];
        if(!(v > 0)){v = 0;}   // also maps NaN to 0
        if(v > 1){v = 1;}
        out[i] = static_cast<unsigned char>(v * 255);
    }
    return out;
}

void Color::normalize(){
    for(int i=0; i<dim; ++i){
        data[i] /= 255;
    }
}

std::ostream& operator << (std::ostream &out, const Color& other){
    out << "R: " << other.get_red() << " G: " << other.get_green() << " B: " << other.get_blue();
    return out;
}

Color random_color(){
    RNG r;
    return Color(r.roll_uniform(0,1), r.roll_uniform(0,1), r.roll_uniform(0,1));
}
