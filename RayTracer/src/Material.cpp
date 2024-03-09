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
    reflective = 1.0;
    transparency = 0.0;
    refractive_index = 1.00029; // defaults to air
    cast_shadow = true;
    mat_color = Color({1,1,1});
}

Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color, std::shared_ptr<Pattern> pat, bool cast_shadow){
    set_ambient(a_ambient);
    set_diffuse(a_diffuse);
    set_specular(a_specular);
    set_shininess(a_shiny);
    reflective = 1.0;
    transparency = 0.0;
    refractive_index = 1.00029; // defaults to air
    this->cast_shadow = cast_shadow;
    mat_color = a_material_color;
    pattern = pat;
}

Material::Material(Color a_material_color){
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
    reflective = 1.0;
    transparency = 0.0;
    refractive_index = 1.00029; // defaults to air
    mat_color = a_material_color;
}

Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color, double reflectance, double transparency, double refractive_index,
    std::shared_ptr<Pattern> pattern, bool cast_shadow){
    set_ambient(a_ambient);
    set_diffuse(a_diffuse);
    set_specular(a_specular);
    set_shininess(a_shiny);
    set_optics(reflectance, transparency);
    set_refractive_index(refractive_index);
    this->cast_shadow = cast_shadow;
    mat_color = a_material_color;
    this->pattern = pattern;
}

double Material::get_ambient() const{return ambient;}
double Material::get_diffuse() const{return diffuse;}
double Material::get_specular() const{return specular;}
double Material::get_shininess() const{return shininess;}
Color Material::get_color() const{return mat_color;}
std::shared_ptr<Pattern> Material::get_pattern() const{return pattern;}
bool Material::is_casting_shadow() const{return cast_shadow;}
double Material::get_reflectance() const {return reflective;};
double Material::get_transparency() const {return transparency;};
double Material::get_refractive_index()const {return refractive_index;};


void Material::set_color(Color c){
    this->mat_color = c;
}

void Material::set_specular(double other){
    if(!is_between(other,0,1)){throw std::invalid_argument("specular must be between 0 and 1");}
    this->specular = other;
}

void Material::set_ambient(double other){
    if(!is_between(other,0,1)){throw std::invalid_argument("ambient must be between 0 and 1");}
    this->ambient = other;
    }

void Material::set_diffuse(double other){
    if(!is_between(other,0,1)){throw std::invalid_argument("diffuse must be between 0 and 1");}
    this->diffuse = other;
}

void Material::set_shininess(double other){
    if(!is_between(other,0,200)){throw std::invalid_argument("shininess must be between 0 and 200");}
    this->shininess = other;
}

void Material::set_pattern(const std::shared_ptr<Pattern> new_pat){this->pattern = new_pat;}
void Material::set_cast_shadow(bool cs){this->cast_shadow = cs;};

void Material::set_reflectance(double r){
    if(!is_between(r,0,1)){throw std::invalid_argument("reflectance must be between 0 and 1");}
    this->reflective = r;
    this->transparency = 1-r;
};

void Material::set_transparency(double t){
    if(!is_between(t,0,1)){throw std::invalid_argument("transparency must be between 0 and 1");}
    this->transparency = t;
    this->reflective = 1-t;
};

void Material::set_refractive_index(double n_i){
    if(n_i <1 ){
        throw std::invalid_argument("refractive index must be at least 1");
    }
    this->refractive_index = n_i;
};

void Material::set_optics(double r, double t){
    if(!is_between(r,0,1)){throw std::invalid_argument("reflectance must be between 0 and 1");}
    if(!is_between(t,0,1)){throw std::invalid_argument("transparency must be between 0 and 1");}
    if( (r+t-1) > glob_resolution ){
        std::invalid_argument("reflectance and transparency must sum to 1");        
    }
    this->transparency = t;
    this->reflective = r;
}

std::ostream& operator << (std::ostream &out, const Material& Mat){
    out <<"Material:" << "\n\tAmbient: " << Mat.get_ambient() << " Diffuse: " << Mat.get_diffuse() << " Specular: " << Mat.get_specular() << " Shininess: " << Mat.get_shininess() << " Shadow: " << Mat.is_casting_shadow() << std::endl;
    out << " Reflective: " << Mat.get_reflectance() << " Transparency: " << Mat.get_transparency() <<  " Refractive Index: " << Mat.get_refractive_index() ;
    out << "\t" << Mat.get_color();
    return out;
}
