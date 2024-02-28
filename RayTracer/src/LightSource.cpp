#include "LightSource.h"

LightSource::LightSource(const Color& intensity,const Tuple& position){
    if(position.type() != TupType::POINT){
        std::invalid_argument("position must be a point in LightSource");
    }
    Intensity = intensity;
    SourceLoc = position;
}

Color LightSource::get_intensity() const{return Intensity;};
Tuple LightSource::get_position() const{return SourceLoc;};
