#include "Camera.h"
#include "Matrix.h"
#include "Color.h"
#include "Constants.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <thread>
#include <vector>

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
    this->aspect_ratio = (static_cast<double>(this->hsize))/( static_cast<double>(this->vsize));
    if(this->aspect_ratio >= 1){
        this->half_width =  half_view;
        this->half_height = static_cast<double>(half_view) / static_cast<double>(this->aspect_ratio);
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
    // Assume that canvas is 1 pixel away from camera
    double half_view = std::tan(this->field_of_view/2.0);
    this->aspect_ratio = (static_cast<double>(this->hsize))/( static_cast<double>(this->vsize));
    if(this->aspect_ratio >= 1){
        this->half_width =  half_view;
        this->half_height = static_cast<double>(half_view) / static_cast<double>(this->aspect_ratio);
    }
    else{
        this->half_width = half_view * this->aspect_ratio;
        this->half_height = half_view;
    }
    this->pixel_size = (this->half_width*2.0)/ (double) this->hsize;
    Tuple forward = pointed_at-camera_loc;
    forward.normalize();
    Tuple upwards_dir = upwards;
    upwards_dir.normalize();
    Tuple left = forward.cross(upwards_dir);
    Tuple true_up = left.cross(forward);
    Matrix orientation(
    {
        {left.get_x(),left.get_y(),left.get_z(),0},
        {true_up.get_x(),true_up.get_y(),true_up.get_z(),0},
        {-forward.get_x(),-forward.get_y(),-forward.get_z(),0},
        {0,0,0,1}
    }
    );
    Matrix trans = MatTranslation(-1.0*camera_loc.get_x(),-1.0*camera_loc.get_y(),-1.0*camera_loc.get_z());
    this->view_trans = orientation*trans;
}

int Camera::get_hsize() const{return this->hsize;}
int Camera::get_vsize() const{return this->vsize;}
double Camera::get_pixel_size() const{return this->pixel_size;};
double Camera::get_field_of_view() const{return this->field_of_view;}
double Camera::get_aspect_ratio() const{return this->aspect_ratio;};
double Camera::get_half_width() const{return this->half_width;};
double Camera::get_half_height() const{return this->half_height;};
Matrix Camera::get_view() const{return this->view_trans;}

void Camera::set_view(const Tuple& camera_loc,const Tuple& pointed_at, const Tuple& upwards){
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

void Camera::render_tile(const World* w, Canvas& image, int x0, int y0, int x1, int y1) const{
    for(int y = y0; y < y1; ++y){
        for(int x = x0; x < x1; ++x){
            image.write_pixel(x, y, w->color_at(ray_for_pixel(x, y)));
        }
    }
}

std::unique_ptr<Canvas> Camera::render(World* w, unsigned int max_depth, unsigned int num_threads, int tile_size, bool verbose){
    if(tile_size <= 0){
        throw std::invalid_argument("tile_size must be positive");
    }
    // Must happen before any threads start: building the BVH writes to the World, and the
    // workers only read from it.
    w->build_bvh();
    if(verbose){std::cout << "BVH Initialized" << std::endl;}
    total_rays = 0;
    auto image = std::make_unique<Canvas>(this->hsize, this->vsize);
    Canvas& canvas = *image;

    const int tiles_x = (this->hsize + tile_size - 1) / tile_size;
    const int tiles_y = (this->vsize + tile_size - 1) / tile_size;
    const int n_tiles = tiles_x * tiles_y;
    std::atomic<int> next_tile{0};

    // Each worker grabs the next unrendered tile until none are left. Tiles never overlap,
    // so every pixel is written by exactly one thread and the canvas needs no locking.
    // Shared state is captured by reference (canvas, next_tile).
    auto worker = [this, w, &canvas, &next_tile, tiles_x, n_tiles, tile_size]{
        thread_rays = 0;
        while(true){
            const int t = next_tile.fetch_add(1);
            if(t >= n_tiles){ break; }
            const int x0 = (t % tiles_x) * tile_size;
            const int y0 = (t / tiles_x) * tile_size;
            render_tile(w, canvas, x0, y0, std::min(x0 + tile_size, this->hsize), std::min(y0 + tile_size, this->vsize));
        }
        total_rays += thread_rays;
    };

    unsigned int threads = num_threads > 0 ? num_threads : std::max(1u, std::thread::hardware_concurrency());
    threads = std::min(threads, static_cast<unsigned int>(n_tiles));
    std::vector<std::thread> pool;
    pool.reserve(threads);
    for(unsigned int i = 0; i < threads; ++i){
        pool.emplace_back(worker);
    }
    for(auto& th : pool){
        th.join();
    }

    if(verbose) std::cout << "Rendered " << n_tiles << " tiles on " << threads << " threads. Total Rays Created: " << total_rays << std::endl;
    return image;
}

std::ostream& operator << (std::ostream& out, const Camera& c){
    out << "\nHsize: "<<   c.get_hsize() << '\n';
    out << "Vsize: "<<   c.get_vsize() << '\n';
    out << "FOV: "<<   c.get_field_of_view() << '\n';
    out << "Pixel Size: "<<   c.get_pixel_size() << '\n';
    out << "Aspect Ratio: "<<   c.get_aspect_ratio() << '\n';
    out << "Half Width: "<<   c.get_half_width() << '\n';
    out << "Half Height: "<<   c.get_half_height() << '\n';
    out << "View: " << c.get_view() << std::endl;
    return out;
}

