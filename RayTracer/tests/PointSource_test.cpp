#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Sphere.h"
#include "Canvas.h"
#include "PointSource.h"

TEST(PointSourceTest,Init){
    PointSource(Color({1,1,1}),Tuple({0,0,0}));
}

TEST(PointSourceTest, Lighting){
    Tuple position({0,0,0}, TupType::POINT);
    Sphere s;
    PointSource ps(Color({1,1,1}),Tuple({0,0,-10}));
    // Straight on
    Tuple camera({0,0,-1});
    Tuple normal({0,0,-1});
    Color result = ps.shade(s.get_material(), position, camera, normal);
    EXPECT_EQ(result, Color(1.9,1.9,1.9));
    // Angled camera
    camera = Tuple({0,std::sqrt(2)/2.0,-std::sqrt(2)/2.0});
    result = ps.shade(s.get_material(), position, camera, normal);
    EXPECT_EQ(result,Color(1,1,1));
    // Angled Light
    camera = Tuple({0,0,-1});
    ps =  PointSource(Color({1,1,1}),Tuple({0,10,-10}));
    result = ps.shade(s.get_material(), position, camera, normal);
    EXPECT_EQ(result, Color(.7364,.7364,.7364));
    // The light! It burns (eye directly in line of sight of light)
    camera = Tuple({0,-std::sqrt(2)/2,-std::sqrt(2)/2});
    result = ps.shade(s.get_material(), position, camera, normal);
    EXPECT_EQ(result, Color(1.6364,1.6364,1.6364));
    // Obscured light
    ps =  PointSource(Color({1,1,1}),Tuple({0,0,10}));
    result = ps.shade(s.get_material(),position, camera,normal);
    EXPECT_EQ(result, Color(0.1,0.1,0.1));
}

TEST(TestImage, ShadedSphere){
    // Place Sphere at origin
    Sphere s(MatScaling(0.25,0.25,0.25), Material(Color(1,0.2,1)));
    // Place light source up, behind and left the camera
    PointSource light_loc(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT));
    // place origin of rays
    double origin_z = -10;
    Tuple origin({0,0, origin_z}, TupType::POINT);
    // Place wall location
    double wall_location = 30;
    // define how big the wall is in X and Y in world space
    double slope = 1.0/origin_z;
    double wall_size = std::ceil(slope*(wall_location-origin_z));
    // Define canvas
    int dim = 100;
    Canvas canvas(dim, dim);
    // Map how big a pixel is in world coordinate units
    double pixel_size = wall_size/(double) dim;
    double half = pixel_size/2.0;
    double half_wall = wall_size/2.0;
    // Aim Rays from ray_origin to centers of each pixel on the wall
    std::vector<double> intersections;
    for(int row=0; row<dim; ++row){
        // Grab center of each pixel in world space, and shift to center of frame
        double world_y = half+pixel_size*row-half_wall;
        for(int col=0; col<dim; ++col){
            double world_x = half-pixel_size*col+half_wall;
            Tuple position({world_x, world_y, wall_location}, TupType::POINT);
            Ray r(origin, normalize(position-origin));
            intersections = s.intersect(r);
            if(intersections.size() != 0){
                Tuple position = r.position(intersections[0]);
                // Calculate what color the pixel should be, given the light source, material, current position, ray direction and normal vector at position
                Color p = light_loc.shade(s.get_material(),
                position,r.get_direction() , s.normal_at(position));
                canvas.write_pixel(row,col,p);
            }
        }
    }
    canvas.save_ppm("ShadedSphere");
}