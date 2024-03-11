#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Sphere.h"
#include "Canvas.h"

TEST(SphereTest, Intersection){
    Ray r(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    Matrix Trans = MatScaling(2,2,2);
    Sphere s(Trans);
    std::vector<double> intersections = s.intersect(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0],3.0);
    EXPECT_EQ(intersections[1],7.0);
    r = Ray(Tuple({0,0,-5}, TupType::POINT),Tuple({0,0,1}));
    Trans = MatTranslation(5,0,0);
    s = Sphere(Trans);
    intersections = s.intersect(r);
    EXPECT_EQ(intersections.size(), 0);
}

TEST(TestImage, Spheres){
    // Define canvas
    int dim = 100;
    Canvas canvas(dim, dim);
    // Place Sphere at origin
    Sphere s(MatScaling(0.25,0.25,0.25));
    // Set sphere color
    Color color = RED;
    // place origin of rays
    double origin_z = -20;
    Tuple origin({0,0, origin_z}, TupType::POINT);
    // Place wall location
    double wall_location = 20;
    // define how big the wall is in X and Y in world space
    double slope = 1.0/origin_z;
    double wall_size = std::ceil(slope*(wall_location-origin_z));
    // Map how big a pixel is in world coordinate units
    double pixel_size = wall_size/(double) dim;
    double half = pixel_size/2.0;
    double half_wall = wall_size/2.0;
    // Aim Rays from ray_origin to centers of each pixel on the wall
    for(int row=0; row<dim; ++row){
        double world_y = half+pixel_size*row-half_wall;
        for(int col=0; col<dim; ++col){
            double world_x = half+pixel_size*col-half_wall;
            Tuple position({world_x, world_y, wall_location}, TupType::POINT);
            Ray r(origin, normalize(position-origin));
            if(s.intersect(r).size() != 0){
                canvas.write_pixel(row,col,color);
            }
        }
    }
    canvas.save_ppm("RedSphere");
}

TEST(SphereTest, Normals){
    // Simple Translation
    Sphere s(MatTranslation(0,1,0));
    Tuple cand({0,1.70711,-0.70711}, TupType::POINT);
    Tuple n = s.normal_at(cand);
    Tuple true_norm({0,.70711, -0.70711});
    EXPECT_EQ(n,true_norm);
    // Combination
    Matrix combo = MatRotateZ(acos(-1)/5.0)*MatScaling(1,0.5,1);
    s = Sphere(combo);
    cand = Tuple({0,std::sqrt(2)/2.0,-std::sqrt(2)/2.0}, TupType::POINT);
    true_norm = Tuple({0,0.97014,-.24254});
}