#include "Material.h"
#include "Pattern.h"
#include <stdexcept>
#include <memory>

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
    cast_shadow = true;
    mat_color = Color({1,1,1});
}

Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color, std::shared_ptr<Pattern> pat, bool cast_shadow){
    if(!is_between(a_ambient,0,1)){throw std::invalid_argument("ambient must be between 0 and 1");}
    if(!is_between(a_diffuse,0,1)){throw std::invalid_argument("diffuse must be between 0 and 1");}
    if(!is_between(a_specular,0,1)){throw std::invalid_argument("specular must be between 0 and 1");}
    if(!is_between(a_shiny,0,200)){throw std::invalid_argument("shininess must be between 0 and 200");}
    ambient = a_ambient;
    diffuse = a_diffuse;
    specular = a_specular;
    shininess =a_shiny;
    this->cast_shadow = cast_shadow;
    mat_color = a_material_color;
    pattern = pat;
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
std::shared_ptr<Pattern> Material::get_pattern() const{return pattern;}
bool Material::is_casting_shadow() const{return cast_shadow;}

void Material::set_color(Color c){this->mat_color = c;}
void Material::set_specular(double other){ this->specular = other;}
void Material::set_ambient(double other){ this->ambient = other;}
void Material::set_diffuse(double other){ this->diffuse = other;}
void Material::set_shininess(double other){ this->shininess = other;}
void Material::set_pattern(const std::shared_ptr<Pattern> new_pat){this->pattern = new_pat;}
void Material::set_cast_shadow(bool cs){this->cast_shadow = cs;};



std::ostream& operator << (std::ostream &out, const Material& Mat){
    out <<"Material:" << "\n\tAmbient: " << Mat.get_ambient() << " Diffuse: " << Mat.get_diffuse() << " Specular: " << Mat.get_specular() << " Shininess: " << Mat.get_shininess() << " Shadow: " << Mat.is_casting_shadow() << std::endl;
    out << "\t" << Mat.get_color();
    return out;
}
