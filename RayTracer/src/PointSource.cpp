#include "PointSource.h"
#include "Color.h"
#include "Tuple.h"
#include "Material.h"
#include "Shape.h"
#include <cmath>

PointSource::PointSource(Color a_intensity, Tuple a_position){
    if(a_position.type() != TupType::POINT){
        std::invalid_argument("a_position must be a point");
    }
    Intensity = a_intensity;
    SourceLoc = a_position;
}

Color PointSource::get_intensity() const{return Intensity;};
Tuple PointSource::get_position() const{return SourceLoc;};

Color PointSource::shade(const Material& mat, const Tuple& position,
const Tuple& camera, const Tuple& normal) const{
    if(position.type() != TupType::POINT){throw std::invalid_argument("position must be a point");}
    if(camera.type() != TupType::VECTOR){throw std::invalid_argument("camera must be a vector");}
    if(normal.type() != TupType::VECTOR){throw std::invalid_argument("normal must be a vector");}
    Color eff_color = mat.get_color()*this->get_intensity(); // Get effective color from light color and material color overlapping
    Tuple light_v = SourceLoc-position;
    light_v.set_type(TupType::VECTOR); // Cast Position from Point to Vector
    light_v.normalize();
    Color ambient = mat.get_ambient()*eff_color;
    Color specular;
    Color diffuse;
    double light_dot_normal = light_v.dot(normal);
    if(light_dot_normal<=0){
        specular = BLACK;
        diffuse = BLACK;
    }
    else{
        double dif_prefactor = (mat.get_diffuse()*light_dot_normal);
        diffuse = eff_color*dif_prefactor;
        Tuple reflected = (-1*light_v).reflect(normal);
        double reflected_dot_camera = camera.dot(reflected);
        if(reflected_dot_camera <=0){
            specular = BLACK;
        }
        else{
            specular = Intensity*mat.get_specular()*std::pow(reflected_dot_camera, mat.get_shininess());
        }
    }
    return ambient+specular+diffuse;
}

void PointSource::print() const{
    std::cout  << "Intensity: " << this->get_intensity() << "\n\t" << "Position: " << this->get_position();
}