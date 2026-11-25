#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"
#include "Tuple.h"
#include "Canvas.h"
#include "Ray.h"
#include "World.h"
#include <atomic>
#include <memory>

class Camera{
private:
    int hsize;
    int vsize;
    double field_of_view;
    double pixel_size;
    double aspect_ratio;
    double half_width;
    double half_height;
    Matrix view_trans;
    // Renders pixels [x0, x1) x [y0, y1) into image. Safe to call concurrently for non-overlapping tiles.
    void render_tile(const World* w, Canvas& image, int x0, int y0, int x1, int y1) const;
public:
    // Total rays cast by the last render(). 
    static inline std::atomic<unsigned long long> total_rays{0};
    // thread local count of rays cast. Aggregate at end
    static inline thread_local unsigned long long thread_rays = 0;
    Camera(int hsize, int vsize, double field_of_view);
    Camera(int hsize, int vsize, double field_of_view, const Tuple& camera_loc, const Tuple& pointed_at, const Tuple& upwards);

    int get_hsize() const;
    int get_vsize() const;
    double get_pixel_size() const;
    double get_field_of_view() const;
    double get_aspect_ratio() const;
    double get_half_width() const;
    double get_half_height() const;
    Matrix get_view() const;

    void set_view(const Tuple& from,const Tuple& to, const Tuple& up);

    void set_view(const Matrix& new_view);

    Ray ray_for_pixel(int x, int y) const;

    // Renders in tile_size x tile_size tiles handed out to worker threads on demand.
    // num_threads = 0 uses one thread per hardware core. verbose = false suppresses the
    // progress output (needed when the terminal itself is the display).
    std::unique_ptr<Canvas> render(World* w, unsigned int depth =20, unsigned int num_threads = 0, int tile_size = 16, bool verbose = true);

    bool operator==(const Camera& other) const;
    bool operator!=(const Camera& other) const;

    friend std::ostream& operator << (std::ostream& out, const Camera& c);
};

#endif
