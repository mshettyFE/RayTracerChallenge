#include "Mandelbulb.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Mandelbulb::Mandelbulb(const Matrix& Transformation, const Material& material, const Shape* parent,
                       double power, int iterations, double surface_epsilon, int max_steps)
    : Shape(Transformation, material, parent, "Mandelbulb"),
      power(power), iterations(iterations), surface_epsilon(surface_epsilon), max_steps(max_steps){
    if(iterations <= 0){throw std::invalid_argument("Mandelbulb iterations must be positive");}
    if(surface_epsilon <= 0){throw std::invalid_argument("Mandelbulb surface_epsilon must be positive");}
    if(max_steps <= 0){throw std::invalid_argument("Mandelbulb max_steps must be positive");}
}

// Standard Mandelbulb distance estimator: iterate z -> z^power + c in spherical coordinates,
double Mandelbulb::distance_estimate(double cx, double cy, double cz) const{
    double x = cx, y = cy, z = cz;
    double dr = 1.0;
    double r = 0.0;
    for(int i = 0; i < iterations; ++i){
        r = std::sqrt(x*x + y*y + z*z);
        if(r > 2.0){break;}   // escaped: the point is outside the set
        if(r < 1e-12){r = 1e-12; break;}   // at the origin, which is deep inside the set
        double theta = std::acos(z / r) * power;
        double phi = std::atan2(y, x) * power;
        double zr = std::pow(r, power);
        dr = std::pow(r, power - 1.0) * power * dr + 1.0;
        x = zr * std::sin(theta) * std::cos(phi) + cx;
        y = zr * std::sin(theta) * std::sin(phi) + cy;
        z = zr * std::cos(theta) + cz;
    }
    // For a good discussion on why to use this distance measure
    //http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-iv-the-holy-grail/
    return 0.5 * std::log(r) * r / dr;
}

std::vector<Impact> Mandelbulb::local_intersect(const Ray &other) const{
    const Tuple o = other.get_origin();
    const Tuple d = other.get_direction();
    const double ox = o[0], oy = o[1], oz = o[2];
    const double dx = d[0], dy = d[1], dz = d[2];

    // Clip the ray to the bounding sphere; a miss there is a miss everywhere.
    const double a = dx*dx + dy*dy + dz*dz;
    const double b = 2.0 * (ox*dx + oy*dy + oz*dz);
    const double c = ox*ox + oy*oy + oz*oz - BOUND_RADIUS*BOUND_RADIUS;
    const double disc = b*b - 4.0*a*c;
    if(disc < 0){return {};}
    const double sq = std::sqrt(disc);
    const double t_exit = (-b + sq) / (2.0*a);
    if(t_exit < 0){return {};}
    double t = std::max((-b - sq) / (2.0*a), 0.0);

    // The estimator gives distances in object space, but t is measured in units of the
    // (possibly scaled) direction vector, so convert with the direction's length.
    const double dir_len = std::sqrt(a);
    auto de_at = [&](double tt){
        return distance_estimate(ox + tt*dx, oy + tt*dy, oz + tt*dz);
    };

    // Rays that start on the surface (shadow and reflection rays leaving a hit point) begin
    // within surface_epsilon of it, and would immediately "hit" their own starting point.
    // Step off the surface first. If that fails, the ray points into the fractal
    const double nudge = 2.0 * surface_epsilon / dir_len;
    int escape = 0;
    while(de_at(t) < surface_epsilon){
        if(++escape > 32){return {Impact(t, this)};}
        t += nudge;
    }

    for(int step = 0; step < max_steps && t <= t_exit; ++step){
        const double dist = de_at(t);
        if(dist < surface_epsilon){
            return {Impact(t, this)};
        }
        t += dist / dir_len;
    }
    return {};
}

// Normal from the gradient of the distance estimator, by central differences.
Tuple Mandelbulb::local_normal_at(const Tuple& pt, const Impact& impt) const{
    const double h = surface_epsilon;
    const double x = pt[0], y = pt[1], z = pt[2];
    Tuple n({distance_estimate(x + h, y, z) - distance_estimate(x - h, y, z),
             distance_estimate(x, y + h, z) - distance_estimate(x, y - h, z),
             distance_estimate(x, y, z + h) - distance_estimate(x, y, z - h)});
    n.normalize();
    return n;
}

void Mandelbulb::verbose_print() const{
    std::cout << name << std::endl;
    std::cout << "Power: " << power << " Iterations: " << iterations << std::endl;
    std::cout << "Transformation:" << std::endl;
    std::cout << get_transform() << std::endl;
    std::cout << mat << std::endl;
    std::cout << "Shape ID: " << id << std::endl;
}

AABB Mandelbulb::bound() const{
    return AABB({-BOUND_RADIUS,-BOUND_RADIUS,-BOUND_RADIUS},{BOUND_RADIUS,BOUND_RADIUS,BOUND_RADIUS},this);
}
