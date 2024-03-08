#ifndef MATERIAL_H
#define MATERIAL_H

#include "Pattern.h"
#include "Color.h"
#include <memory>

class Pattern;

class Material{
private:
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    bool cast_shadow;
    Color mat_color;
    std::shared_ptr<Pattern> pattern;
    bool is_between(double value, double min, double max);
public:
    Material();
    Material(double a_ambient, double a_diffuse, double a_specular,
    double a_shiny, Color a_material_color,    std::shared_ptr<Pattern> pattern=nullptr, bool cast_shadow=true);
    Material(Color a_material_color);

    double get_ambient() const;
    void set_ambient(double other);

    double get_diffuse() const;
    void set_diffuse(double other);

    double get_specular() const;
    void set_specular(double other);

    double get_shininess() const;
    void set_shininess(double other);

    bool is_casting_shadow() const;
    void set_cast_shadow(bool cs);

    Color get_color() const;
    void set_color(Color c);

    std::shared_ptr<Pattern> get_pattern() const;
    void set_pattern(const std::shared_ptr<Pattern> new_pat);

    friend std::ostream& operator << (std::ostream &out, const Material& Material);
};

#endif