#ifndef MANDELBULB_H
#define MANDELBULB_H

#include "Shape.h"

// The Mandelbulb fractal, rendered by sphere tracing ("ray marching") a distance estimator.
// In object space it fits inside a sphere of radius BOUND_RADIUS centred on the origin, with
// its axis of symmetry along z.
// Equations (White and Nylander's spherical-coordinate power formula): https://en.wikipedia.org/wiki/Mandelbulb
//
// Only the first hit along a ray is reported, so the shape is opaque: reflection works, but
// refraction does not (the tracer needs entry/exit pairs, which a fractal doesn't have).
class Mandelbulb : public Shape{
private:
    double power;
    int iterations;
    double surface_epsilon;
    int max_steps;
    // If power is 2^k (2, 4, 8, ...), k; otherwise 0. Selects the trig-free distance estimate.
    int angle_doublings;

    double distance_estimate_power_of_two(double x, double y, double z) const;

    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
public:
    static constexpr double BOUND_RADIUS = 1.2;

    // power: the classic bulb is 8. iterations: more gives finer detail but costs more per step.
    // surface_epsilon: how close (object space) a march must get to count as a hit. It also sets
    // the finest detail resolved: much smaller than a pixel's footprint just adds speckle noise
    // (1e-4 looked grainy at 400x300; 1e-3 was clean and faster).
    Mandelbulb(const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), const Shape* parent=nullptr,
               double power=8.0, int iterations=10, double surface_epsilon=1e-3, int max_steps=300);

    // Lower bound on the distance from object-space point (x, y, z) to the surface. <= 0 inside.
    // Uses the trig-free iteration when power is a power of two, otherwise distance_estimate_trig.
    double distance_estimate(double x, double y, double z) const;
    // The same estimate computed directly from the spherical-coordinate formula. Works for any
    // power; public so tests can check the fast path against it.
    double distance_estimate_trig(double x, double y, double z) const;

    double get_power() const{return power;}
    int get_iterations() const{return iterations;}
    double get_surface_epsilon() const{return surface_epsilon;}

    void verbose_print() const override;
    AABB bound() const override;
};

#endif
