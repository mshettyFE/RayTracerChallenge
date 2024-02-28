#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"
#include "Tuple.h"
#include "Canvas.h"
#include "World.h"
#include "Ray.h"

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
public:
    Camera(int hsize, int vsize, double field_of_view);
    Camera(int hsize, int vsize, double field_of_view, const Tuple& camera_loc, const Tuple& pointed_at, const Tuple& upwards);

    int get_hsize() const;
    int get_vsize() const;
    double get_pixel_size() const;
    double get_field_of_view() const;
    Matrix get_view() const;

    void set_view(const Matrix& new_view);

    Ray ray_for_pixel(int x, int y) const;

    Canvas render(World w) const;

    bool operator==(const Camera& other) const;
    bool operator!=(const Camera& other) const;

};

#endif