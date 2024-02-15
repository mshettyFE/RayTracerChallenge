#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

class Material{
private:
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    Color mat_color;
    bool is_between(double value, double min, double max);
public:
    Material();
    Material(double a_ambient, double a_diffuse, double a_specular,
    double a_shiny, Color a_material_color);
    Material(Color a_material_color);

    double get_ambient() const;
    double get_diffuse() const;
    double get_specular() const;
    double get_shininess() const;
    Color get_color() const;

};

#endif