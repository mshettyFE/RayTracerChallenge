#include "Material.h"
#include <stdexcept>

bool Material::is_between(double value, double min, double max){
    if(value < min){
        return false;
    }
    if(value > max){
        return false;
    }
    return true;
}

Material::Material(){
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
    mat_color = Color({1,1,1});
}

Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color){
    if(!is_between(a_ambient,0,1)){throw std::invalid_argument("ambient must be between 0 and 1");}
    if(!is_between(a_diffuse,0,1)){throw std::invalid_argument("diffuse must be between 0 and 1");}
    if(!is_between(a_specular,0,1)){throw std::invalid_argument("specular must be between 0 and 1");}
    if(!is_between(a_shiny,0,200)){throw std::invalid_argument("shininess must be between 0 and 200");}
    ambient = a_ambient;
    diffuse = a_diffuse;
    specular = a_specular;
    shininess =a_shiny;
    mat_color = a_material_color;
}

Material::Material(Color a_material_color){
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
    mat_color = a_material_color;
}


double Material::get_ambient() const{return ambient;}
double Material::get_diffuse() const{return diffuse;}
double Material::get_specular() const{return specular;}
double Material::get_shininess() const{return shininess;}
Color Material::get_color() const{return mat_color;}
