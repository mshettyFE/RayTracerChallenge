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
    void set_ambient(double other);

    double get_diffuse() const;
    void set_diffuse(double other);

    double get_specular() const;
    void set_specular(double other);

    double get_shininess() const;
    void set_shininess(double other);

    Color get_color() const;
    void set_color(Color c);

    friend std::ostream& operator << (std::ostream &out, const Material& Material);
};

#endif