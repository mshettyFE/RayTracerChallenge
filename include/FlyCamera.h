#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "Camera.h"
#include "Tuple.h"

// A free-flying camera driven by single key presses: each key press is one discrete step.
// Orientation is yaw (turning left/right about the world y axis; 0 looks along +z) and pitch
// (looking up/down). Positive yaw turns right, positive pitch looks up.
//
//   w/s: move forward/back along the view direction    a/d: strafe left/right (horizontal)
//   j/l: turn left/right                                i/k: look up/down
class FlyCamera{
private:
    Tuple position;
    double yaw;
    double pitch;
    double move_step;
    double turn_step;
public:
    // Pitch is kept just short of straight up/down, where the camera's "up" would be undefined.
    static constexpr double MAX_PITCH = 1.55;   // radians, about 89 degrees

    FlyCamera(const Tuple& position, double yaw=0.0, double pitch=0.0, double move_step=0.15, double turn_step=0.05);

    // Applies one key press. Returns true if the view changed (the key was a movement key).
    bool handle_key(char key);

    // Unit vector the camera looks along.
    Tuple forward() const;
    // Horizontal unit vector to the camera's right.
    Tuple right() const;

    Camera make_camera(int hsize, int vsize, double field_of_view) const;

    const Tuple& get_position() const{return position;}
    double get_yaw() const{return yaw;}
    double get_pitch() const{return pitch;}
};

#endif
