#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "Sphereflake.h"

struct Ball{ double x, y, z, r; };
static std::vector<Ball> balls(const Group& flake){
    std::vector<Ball> out;
    for(int i = 0; i < flake.get_total_children(); ++i){
        Matrix t = flake.get_child(i)->get_transform();   // translation * uniform scaling
        out.push_back({t(0, 3), t(1, 3), t(2, 3), t(0, 0)});
    }
    return out;
}

TEST(Sphereflake, SphereCount){
    for(int levels = 0; levels <= 3; ++levels){
        EXPECT_EQ(make_sphereflake(levels, Material())->get_total_children(), sphereflake_sphere_count(levels));
    }
    EXPECT_EQ(sphereflake_sphere_count(0), 1);
    EXPECT_EQ(sphereflake_sphere_count(4), 7381);
}

// Every pair of spheres is at most touching: none overlap, including a child and the spheres it
// grew from (which would happen if a child pointed back towards its parent).
TEST(Sphereflake, SpheresNeverOverlap){
    auto b = balls(*make_sphereflake(3, Material()));
    for(size_t i = 0; i < b.size(); ++i){
        for(size_t j = i + 1; j < b.size(); ++j){
            double d = std::sqrt((b[i].x-b[j].x)*(b[i].x-b[j].x) + (b[i].y-b[j].y)*(b[i].y-b[j].y) + (b[i].z-b[j].z)*(b[i].z-b[j].z));
            ASSERT_GE(d, b[i].r + b[j].r - 1e-9) << "spheres " << i << " and " << j << " overlap";
        }
    }
}

TEST(Sphereflake, FitsWithinRadiusTwo){
    for(const Ball& s : balls(*make_sphereflake(4, Material()))){
        EXPECT_LE(std::sqrt(s.x*s.x + s.y*s.y + s.z*s.z) + s.r, 2.0);
    }
}
