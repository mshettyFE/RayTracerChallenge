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
}

Color::Color(double red, double green, double blue){
    data.clear();
    data.push_back(red);
    data.push_back(green);
    data.push_back(blue);
    data.push_back(TupType::VECTOR);
    dim = 3;
}

Color::Color(int hex){
    data.clear();
    double red = RED_CHANNEL(hex);
    double green = GREEN_CHANNEL(hex);
    double blue = BLUE_CHANNEL(hex);
    data.push_back(red/255.0);
    data.push_back(green/255.0);
    data.push_back(blue/255.0);
    data.push_back(TupType::VECTOR);
    dim = 3;
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
    if((this->dim != other.dim) && (other.dim !=4)){
        throw std::invalid_argument("Dimensions of color don't match");
    }
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