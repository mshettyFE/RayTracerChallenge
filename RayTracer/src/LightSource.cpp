#include "LightSource.h"

LightSource::LightSource(const Color& intensity,const Tuple& position, std::string light_type){
    if(position.type() != TupType::POINT){
        std::invalid_argument("position must be a point in LightSource");
    }
    Intensity = intensity;
    SourceLoc = position;
    name = light_type;
}

std::string LightSource::get_name() const{
    return this->name;
}

Color LightSource::get_intensity() const{return Intensity;};
Tuple LightSource::get_position() const{return SourceLoc;};
