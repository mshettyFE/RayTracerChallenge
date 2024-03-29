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
    reflective = 0.0;
    transparency = 0.0;
    refractive_index = VACUUM; 
    cast_shadow = true;
    mat_color = Color({1,1,1});
    pattern = nullptr;
}

Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color, std::shared_ptr<Pattern> pat, bool cast_shadow){
    set_ambient(a_ambient);
    set_diffuse(a_diffuse);
    set_specular(a_specular);
    set_shininess(a_shiny);
    reflective = 0.0;
    transparency = 0.0;
    refractive_index = VACUUM;
    this->cast_shadow = cast_shadow;
    mat_color = a_material_color;
    pattern = pat;
}

Material::Material(Color a_material_color){
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
    reflective = 0.0;
    transparency = 0.0;
    refractive_index = VACUUM;
    mat_color = a_material_color;
    pattern = nullptr;
    this->cast_shadow = true;
}

Material::Material(Color a_material_color, double reflectance, double tranparency, double refractive_index,std::shared_ptr<Pattern> pattern, bool cast_shadow){
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
    set_reflectance(reflectance);
    set_transparency(tranparency);
    set_refractive_index(refractive_index);
    mat_color = a_material_color;
    this->pattern = pattern;
    this->cast_shadow = cast_shadow;
}


Material::Material(double a_ambient, double a_diffuse, double a_specular,
double a_shiny, Color a_material_color, double reflectance, double transparency, double refractive_index,
    std::shared_ptr<Pattern> pattern, bool cast_shadow){
    set_ambient(a_ambient);
    set_diffuse(a_diffuse);
    set_specular(a_specular);
    set_shininess(a_shiny);
    set_reflectance(reflectance);
    set_transparency(transparency);
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
    if(r<0){throw std::invalid_argument("reflectance must be at least 0");}
    this->reflective = r;
};

void Material::set_transparency(double t){
    if(t<0){throw std::invalid_argument("transparency must be at least 0");}
    this->transparency = t;
};

void Material::set_refractive_index(double n_i){
    if(n_i <1 ){
        throw std::invalid_argument("refractive index must be at least 1");
    }
    this->refractive_index = n_i;
};

std::ostream& operator << (std::ostream &out, const Material& Mat){
    out <<"Material:" << "\n\tAmbient: " << Mat.get_ambient() << " Diffuse: " << Mat.get_diffuse() << " Specular: " << Mat.get_specular() << " Shininess: " << Mat.get_shininess() << " Shadow: " << Mat.is_casting_shadow() << std::endl;
    out << " Reflective: " << Mat.get_reflectance() << " Transparency: " << Mat.get_transparency() <<  " Refractive Index: " << Mat.get_refractive_index() ;
    out << "\t" << Mat.get_color();
    return out;
}
