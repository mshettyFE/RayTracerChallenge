#include "Camera.h"
#include "Matrix.h"
#include "Color.h"
#include "Constants.h"
#include <stdexcept>
#include <cmath>

Camera::Camera(int hsize, int vsize, double field_of_view){
    if(hsize <=0){
        throw std::invalid_argument("invalid horizontal width of camera");
    }
    this->hsize = hsize;
    if(vsize <=0){
        throw std::invalid_argument("invalid vertical width of camera");
    }
    this->vsize = vsize;
    if(field_of_view <=0){
        throw std::invalid_argument("invalid FOV of camera");
    }
    // Assume that canvas is 1 pixel away from camera
    this->field_of_view = field_of_view;
    this->view_trans = MatIdentity(4);
    double half_view = std::tan(this->field_of_view/2.0);
    this->aspect_ratio = ((double)  this->hsize)/( (double) this->vsize);
    if(this->aspect_ratio >= 1){
        this->half_width =  half_view;
        this->half_height = half_view / this->aspect_ratio;
    }
    else{
        this->half_width = half_view * this->aspect_ratio;
        this->half_height = half_view;
    }
    this->pixel_size = (this->half_width*2.0)/ (double) this->hsize;
}

Camera::Camera(int hsize, int vsize, double field_of_view, const Tuple& camera_loc, const Tuple& pointed_at, const Tuple& upwards){
    if(hsize <=0){
        throw std::invalid_argument("invalid horizontal width of camera");
    }
    this->hsize = hsize;
    if(vsize <=0){
        throw std::invalid_argument("invalid vertical width of camera");
    }
    this->vsize = vsize;
    if(field_of_view <=0){
        throw std::invalid_argument("invalid FOV of camera");
    }
    this->field_of_view = field_of_view;
    if(camera_loc.type() != TupType::POINT){
        throw std::invalid_argument("Camera location must be a point");
    }
    if(pointed_at.type() != TupType::POINT){
        throw std::invalid_argument("World location camera is aimed at must be a point");
    }
    if(upwards.type() != TupType::VECTOR){
        throw std::invalid_argument("The upwards direction must be a vector");
    }
    Tuple forward = pointed_at-camera_loc;
    forward.normalize();
    Tuple upwards_dir = upwards;
    upwards_dir.normalize();
    Tuple left = forward.cross(upwards_dir);
    Tuple true_up = left.cross(forward);
    Matrix orientation(
    {
        {left[0],left[1],left[2],0},
        {true_up[0],true_up[1],true_up[2],0},
        {-forward[0],-forward[1],-forward[2],0},
        {0,0,0,1}
    }
    );
    Matrix trans = MatTranslation(-1.0*camera_loc[0],-1.0*camera_loc[1],-1.0*camera_loc[2]);
    this->view_trans = orientation*trans;
}

int Camera::get_hsize() const{return hsize;}
int Camera::get_vsize() const{return vsize;}
double Camera::get_pixel_size() const{return pixel_size;};
double Camera::get_field_of_view() const{return field_of_view;}
Matrix Camera::get_view() const{return view_trans;}

void Camera::set_view(const Matrix& new_view){
    this->view_trans = new_view;
}

bool Camera::operator==(const Camera& other) const{
    if(this->hsize != other.get_hsize()){return false;}
    if(this->vsize != other.get_vsize()){return false;}
    if(std::abs(this->field_of_view-other.get_field_of_view()) > glob_resolution){return false;}
    if(this->view_trans != other.get_view()){return false;}
    return true;
}
bool Camera::operator!=(const Camera& other) const{
    if(this->hsize != other.get_hsize()){return true;}
    if(this->vsize != other.get_vsize()){return true;}
    if(std::abs(this->field_of_view-other.get_field_of_view()) > glob_resolution){return true;}
    if(this->view_trans != other.get_view()){return true;}
    return false;
}

Ray Camera::ray_for_pixel(int px, int py) const{
    // Get center of from edge to center
    double x_offset = (px+0.5)*this->pixel_size;
    double y_offset = (py+0.5)*this->pixel_size;
    // +x is to the *left* since camera looks towards -z
    double world_x = this->half_width-x_offset;
    double world_y = this->half_height-y_offset;
    Tuple world_pt = Tuple({world_x, world_y, -1}, TupType::POINT);
    Tuple pixel = this->view_trans.Inverse()*world_pt;
    Tuple origin = Tuple({0,0,0}, TupType::POINT);
    origin = this->view_trans.Inverse()*origin;
    Tuple direction = pixel-origin;
    direction.normalize();
    return Ray(origin, direction);
}

Canvas Camera::render(World w) const{
    Canvas image = Canvas(this->hsize, this->vsize);
    for(int y=0; y<this->vsize-1; ++y){
        for(int x=0; x<this->hsize-1; ++x){
            Ray r = ray_for_pixel(x,y);
            Color c = w.color_at(r);
            image.write_pixel(x,y,c);
        }
    }
    return image;
}
