#include "Sphereflake.h"
#include "Sphere.h"
#include <cmath>
#include <stdexcept>

namespace {

// Children around the equator, and children tilted up towards the parent's outward axis.
constexpr int EQUATOR_CHILDREN = 6;
constexpr int UPPER_CHILDREN = 3;
// Elevation of the upper children above the equator. Chosen so that no two children overlap:
// the closest pair (an upper and an equator child) are ~56 degrees apart, more than the ~29
// degrees two tangent spheres of a third the radius need at this distance.
const double UPPER_ELEVATION = 50.0 * pi / 180.0;

// Adds the sphere at center with the given radius, then its children. axis points away from the
// parent: children are placed around it and above it, never back towards the parent.
void add_sphere(Group& flake, const Tuple& center, double radius, const Tuple& axis, int levels_left, const Material& material){
    flake.add_child(std::make_unique<Sphere>(MatTranslation(center.get_x(), center.get_y(), center.get_z()) * MatScaling(radius, radius, radius), material));
    if(levels_left == 0){return;}

    // Two unit vectors perpendicular to axis, to measure the children's angles around it.
    const Tuple helper = (std::abs(axis.get_y()) < 0.9) ? GenVec(0, 1, 0) : GenVec(1, 0, 0);
    const Tuple u = normalize(helper.cross(axis));
    const Tuple v = axis.cross(u);

    const double child_radius = radius / 3.0;
    const double distance = radius + child_radius;   // tangent to this sphere
    auto add_child_at = [&](double azimuth, double elevation){
        Tuple direction = axis * std::sin(elevation) + (u * std::cos(azimuth) + v * std::sin(azimuth)) * std::cos(elevation);
        direction.normalize();
        add_sphere(flake, center + direction * distance, child_radius, direction, levels_left - 1, material);
    };
    for(int i = 0; i < EQUATOR_CHILDREN; ++i){
        add_child_at(2.0 * pi * i / EQUATOR_CHILDREN, 0.0);
    }
    // At 30, 150 and 270 degrees: midway between neighbouring equator children (which sit at
    // multiples of 60), so each upper child is nestled between two of them.
    for(int i = 0; i < UPPER_CHILDREN; ++i){
        add_child_at(2.0 * pi * i / UPPER_CHILDREN + pi / EQUATOR_CHILDREN, UPPER_ELEVATION);
    }
}

} // namespace

std::unique_ptr<Group> make_sphereflake(int levels, const Material& material, const Matrix& transform){
    if(levels < 0){
        throw std::invalid_argument("sphereflake levels must be non-negative");
    }
    auto flake = std::make_unique<Group>(transform);
    add_sphere(*flake, GenPoint(0, 0, 0), 1.0, GenVec(0, 1, 0), levels, material);
    return flake;
}

long sphereflake_sphere_count(int levels){
    long count = 0, at_level = 1;
    for(int level = 0; level <= levels; ++level){
        count += at_level;
        at_level *= EQUATOR_CHILDREN + UPPER_CHILDREN;
    }
    return count;
}
