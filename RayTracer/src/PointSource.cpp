#include "PointSource.h"
#include "LightSource.h"
#include "Color.h"
#include "Tuple.h"
#include "Material.h"
#include "Shape.h"
#include <cmath>

PointSource::PointSource(const Color& a_intensity,const Tuple& a_position)  : LightSource(a_intensity, a_position, "PointSource" ){}

Color PointSource::shade(const std::shared_ptr<Shape> obj, const Tuple& position,
        const Tuple& camera, const Tuple& normal, bool in_shadow) const{
    if(position.type() != TupType::POINT){throw std::invalid_argument("position must be a point");}
    if(camera.type() != TupType::VECTOR){throw std::invalid_argument("camera must be a vector");}
    if(normal.type() != TupType::VECTOR){throw std::invalid_argument("normal must be a vector");}
    if(obj==nullptr){throw std::invalid_argument("Can't shade a nullptr object");}
    Color cur_color;
    if(obj->get_material().get_pattern()==nullptr){
        cur_color = obj->get_material().get_color();
    }
    else{
        cur_color = obj->get_material().get_pattern()->at_object(position,obj);
    }
    Color eff_color = cur_color*this->get_intensity(); // Get effective color from light color and material color overlapping
    Tuple light_v = SourceLoc-position;
    light_v.set_type(TupType::VECTOR); // Cast Position from Point to Vector
    light_v.normalize();
    Color ambient = obj->get_material().get_ambient()*eff_color;
    Color specular = BLACK;
    Color diffuse = BLACK;
    if(!in_shadow){
        double light_dot_normal = light_v.dot(normal);
        if(light_dot_normal<=0){
            specular = BLACK;
            diffuse = BLACK;
        }
        else{
            double dif_prefactor = (obj->get_material().get_diffuse()*light_dot_normal);
            diffuse = eff_color*dif_prefactor;
            Tuple reflected = (-1*light_v).reflect(normal);
            double reflected_dot_camera = camera.dot(reflected);
            if(reflected_dot_camera <=0){
                specular = BLACK;
            }
            else{
                specular = Intensity*obj->get_material().get_specular()*std::pow(reflected_dot_camera, obj->get_material().get_shininess());
            }
        }
    }
    return ambient+specular+diffuse;
}


void PointSource::print() const{
    std::cout  << "Intensity: " << this->get_intensity() << "\n\t" << "Position: " << this->get_position();
}