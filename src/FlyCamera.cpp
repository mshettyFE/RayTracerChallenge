#include "FlyCamera.h"
#include <algorithm>
#include <cctype>
#include <cmath>

FlyCamera::FlyCamera(const Tuple& position, double yaw, double pitch, double move_step, double turn_step)
    : position(position), yaw(yaw), pitch(std::clamp(pitch, -MAX_PITCH, MAX_PITCH)),
      move_step(move_step), turn_step(turn_step){}

Tuple FlyCamera::forward() const{
    return GenVec(std::cos(pitch) * std::sin(yaw), std::sin(pitch), std::cos(pitch) * std::cos(yaw));
}

Tuple FlyCamera::right() const{
    // up x forward, flattened to the horizontal plane: +x when yaw is 0.
    return GenVec(std::cos(yaw), 0.0, -std::sin(yaw));
}

bool FlyCamera::handle_key(char key){
    switch(std::tolower(static_cast<unsigned char>(key))){
        case 'w': position = position + forward() * move_step; return true;
        case 's': position = position - forward() * move_step; return true;
        case 'd': position = position + right() * move_step; return true;
        case 'a': position = position - right() * move_step; return true;
        case 'l': yaw += turn_step; return true;
        case 'j': yaw -= turn_step; return true;
        case 'i': pitch = std::min(pitch + turn_step, MAX_PITCH); return true;
        case 'k': pitch = std::max(pitch - turn_step, -MAX_PITCH); return true;
        default: return false;
    }
}

Camera FlyCamera::make_camera(int hsize, int vsize, double field_of_view) const{
    return Camera(hsize, vsize, field_of_view, position, position + forward(), GenVec(0, 1, 0));
}
