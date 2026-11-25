#ifndef SPHEREFLAKE_H
#define SPHEREFLAKE_H

#include "Group.h"
#include "Material.h"
#include "Matrix.h"
#include <memory>

// The "sphereflake": a classic ray tracer benchmark scene, after Eric Haines' "balls" scene from
// the Standard Procedural Databases (1987). A sphere carries 9 smaller spheres, each a third of
// its radius and touching it: 6 around its "equator" and 3 tilted towards the side facing away
// from its parent. Each of those carries 9 more, and so on.
//
// The root sphere has radius 1 and is centred at the origin with its children's "up" along +y;
// the whole flake fits within radius 2. Use transform to place it.
// Sphere count: 1 + 9 + 9^2 + ... + 9^levels (levels 4 = 7,381; levels 5 = 66,430).
std::unique_ptr<Group> make_sphereflake(int levels, const Material& material, const Matrix& transform = MatIdentity(4));

long sphereflake_sphere_count(int levels);

#endif
