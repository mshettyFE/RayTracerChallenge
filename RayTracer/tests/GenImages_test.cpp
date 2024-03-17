#include "World.h"
#include <gtest/gtest.h>
#include "Sphere.h"
#include "PointSource.h"
#include "Camera.h"
#include "Plane.h"
#include "GradientPattern.h"
#include "Color.h"
#include "Checkers.h"
#include "OBJParser.h"
#include <random>
#include <vector>
#include <tuple>

TEST(GenImage, OnlySpheres){
// floor
    Material floor_mat = Material();
    floor_mat.set_color(Color({1, 0.9, 0.9}));
    floor_mat.set_specular(0);
    Sphere floor = Sphere(MatScaling(10,0.01,10), floor_mat);
// left wall
    Material wall_mat = floor_mat;
    Matrix left_wall_transformation = MatTranslation(0,0,5)*MatRotateY(-pi/4.0)*MatRotateX(pi/2.0)*MatScaling(10,0.01,10);
    Sphere lwall = Sphere(left_wall_transformation, wall_mat);
// right wall
    Matrix right_wall_transformation = MatTranslation(0,0,5)*MatRotateY(pi/4.0)*MatRotateX(pi/2.0)*MatScaling(10,0.01,10);
    Sphere rwall = Sphere(right_wall_transformation, wall_mat);
// middle sphere
    Matrix middle_transform = MatTranslation(-0.5,1,0.5);
    Material middle_mat = Material();
    middle_mat.set_diffuse(0.7);
    middle_mat.set_specular(0.3);
    middle_mat.set_color(Color({0.1,1,0.5}));
    Sphere middle = Sphere(middle_transform, middle_mat);
// right sphere
    Matrix right_transform = MatTranslation(1.5,0.5,-0.5)*MatScaling(0.5,0.5,0.5);
    Material right_mat = middle_mat;
    right_mat.set_color(Color({0.5,1,0.1}));
    right_mat.set_diffuse(0.7);
    right_mat.set_specular(0.3);
    Sphere right = Sphere(right_transform, right_mat);
// smallest sphere
    Matrix smallest_trans = MatTranslation(-1.5,0.33,-0.75)*MatScaling(0.33,0.33,0.33);
    Material smallest_mat = Material();
    smallest_mat.set_diffuse(0.7);
    smallest_mat.set_specular(0.3);
    smallest_mat.set_color(Color({0.1,0.8,0.1}));
    Sphere smallest = Sphere(smallest_trans, smallest_mat);
// Light source
    PointSource ps(WHITE, Tuple({10,10,-10}, TupType::POINT));
// camera
    Tuple from = Tuple({0,1.5,-5}, TupType::POINT);
    Tuple to = Tuple({0,1,0}, TupType::POINT);
    Tuple up = Tuple({1,0,0});
    Camera c(100,100, pi/3.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Sphere>(std::move(floor)));
    w.add_shape(std::make_unique<Sphere>(std::move(lwall)));
    w.add_shape(std::make_unique<Sphere>(std::move(rwall)));
    w.add_shape(std::make_unique<Sphere>(std::move(middle)));
    w.add_shape(std::make_unique<Sphere>(std::move(right)));
    w.add_shape(std::make_unique<Sphere>(std::move(smallest)));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    std::unique_ptr<Canvas> img = c.render(&w);
    img->save_ppm("Scene");
}

TEST(GenImage,ShadedSphereRedux){
    // unit sphere at origin
    Sphere s(MatIdentity(4), Material(Color(1,0.2,1)));
    // Place light source up, behind and left the camera
    PointSource light_loc(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT));
    Tuple from = Tuple({0,0,-3}, TupType::POINT);
    Tuple to = Tuple({0,0,0}, TupType::POINT);
    Tuple up = Tuple({0,1,0});
    Camera c(100,100, pi/4.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Sphere>(std::move(s)));
    w.add_source(std::make_unique<PointSource>(std::move(light_loc)));
    std::unique_ptr<Canvas> img = c.render(&w);
    img->save_ppm("ShadedSphereCamera");
}

TEST(GenImage, HotPink){
    int width = 400;
    int height = 480;
    Canvas c(width, height);
    for(int i=0; i<width; ++i){
        for(int j=0; j<height; ++j){
            c.write_pixel(i,j,Color(0xFE019A));
        }
    }
    c.save_ppm("HotPink");
}

TEST(GenImage, GradPatternTest){
// floor
    Material floor_mat = Material();
    floor_mat.set_color(Color({1, 0.9, 0.9}));
    floor_mat.set_specular(0);
    Plane floor = Plane(MatScaling(10,0.01,10), floor_mat);
// middle sphere
    Matrix middle_transform = MatTranslation(-0.5,1,0.5);
    Material middle_mat = Material();
    middle_mat.set_diffuse(0.7);
    middle_mat.set_specular(0.3);
    middle_mat.set_color(Color({0.1,1,0.5}));
    middle_mat.set_pattern(std::make_unique<GradientPattern>(GradientPattern(WHITE,BLACK, MatScaling(2,2,2))));
    Sphere middle = Sphere(middle_transform, middle_mat);
// right sphere
    Matrix right_transform = MatTranslation(1.5,0.5,-0.5)*MatScaling(0.5,0.5,0.5);
    Material right_mat = middle_mat;
    right_mat.set_color(Color({0.5,1,0.1}));
    right_mat.set_diffuse(0.7);
    right_mat.set_specular(0.3);
    Sphere right = Sphere(right_transform, right_mat);
// smallest sphere
    Matrix smallest_trans = MatTranslation(-1.5,0.33,-0.75)*MatScaling(0.33,0.33,0.33);
    Material smallest_mat = Material();
    smallest_mat.set_diffuse(0.7);
    smallest_mat.set_specular(0.3);
    smallest_mat.set_color(Color({0.1,0.8,0.1}));
    Sphere smallest = Sphere(smallest_trans, smallest_mat);
// Light source
    PointSource ps(WHITE, Tuple({-10,10,-10}, TupType::POINT));
// camera
    Tuple from = Tuple({0,1.5,-5}, TupType::POINT);
    Tuple to = Tuple({0,1,0}, TupType::POINT);
    Tuple up = Tuple({1,0,0});
    Camera c(100,100, pi/3.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Plane>(std::move(floor)));
    w.add_shape(std::make_unique<Sphere>(std::move(middle)));
    w.add_shape(std::make_unique<Sphere>(std::move(right)));
    w.add_shape(std::make_unique<Sphere>(std::move(smallest)));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    
    std::unique_ptr<Canvas> img = c.render(&w);
    img->save_ppm("GradPlaneScene");
}

TEST(GenImage, Planes){
// floor
    Material floor_mat = Material();
    floor_mat.set_color(Color({1, 0.9, 0.9}));
    floor_mat.set_specular(0);
    Plane floor = Plane(MatScaling(10,0.01,10), floor_mat);
// middle sphere
    Matrix middle_transform = MatTranslation(-0.5,1,0.5);
    Material middle_mat = Material();
    middle_mat.set_diffuse(0.7);
    middle_mat.set_specular(0.3);
    middle_mat.set_color(Color({0.1,1,0.5}));
    Sphere middle = Sphere(middle_transform, middle_mat);
// right sphere
    Matrix right_transform = MatTranslation(1.5,0.5,-0.5)*MatScaling(0.5,0.5,0.5);
    Material right_mat = middle_mat;
    right_mat.set_color(Color({0.5,1,0.1}));
    right_mat.set_diffuse(0.7);
    right_mat.set_specular(0.3);
    Sphere right = Sphere(right_transform, right_mat);
// smallest sphere
    Matrix smallest_trans = MatTranslation(-1.5,0.33,-0.75)*MatScaling(0.33,0.33,0.33);
    Material smallest_mat = Material();
    smallest_mat.set_diffuse(0.7);
    smallest_mat.set_specular(0.3);
    smallest_mat.set_color(Color({0.1,0.8,0.1}));
    Sphere smallest = Sphere(smallest_trans, smallest_mat);
// Light source
    PointSource ps(WHITE, Tuple({-10,10,-10}, TupType::POINT));
// camera
    Tuple from = Tuple({0,1.5,-5}, TupType::POINT);
    Tuple to = Tuple({0,1,0}, TupType::POINT);
    Tuple up = Tuple({1,0,0});
    Camera c(100,100, pi/3.0, from, to, up );
    World w;
    w.add_shape(std::make_unique<Plane>(std::move(floor)));
    w.add_shape(std::make_unique<Sphere>(std::move(middle)));
    w.add_shape(std::make_unique<Sphere>(std::move(right)));
    w.add_shape(std::make_unique<Sphere>(std::move(smallest)));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    
    std::unique_ptr<Canvas> img = c.render(&w);
    img->save_ppm("PlaneScene");
}

TEST(GenImage, ShadedSphere){
    // Place Sphere at origin
    Sphere s(MatScaling(0.25,0.25,0.25), Material(Color(1,0.2,1)));
    // Place light source up, behind and left the camera
    PointSource light_loc(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT));
    // place origin of rays
    double origin_z = -1000;
    Tuple origin({0,0, origin_z}, TupType::POINT);
    // Place wall location
    double wall_location = 200;
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
    std::vector<Impact> intersections;
    for(int row=0; row<dim; ++row){
        // Grab center of each pixel in world space, and shift to center of frame
        double world_y = half+pixel_size*row-half_wall;
        for(int col=0; col<dim; ++col){
            double world_x = half-pixel_size*col+half_wall;
            Tuple position({world_x, world_y, wall_location}, TupType::POINT);
            Ray r(origin, normalize(position-origin));
            intersections = s.intersect(r);
            if(intersections.size() != 0){
                Tuple position = r.position(intersections[0].get_t());
                // Calculate what color the pixel should be, given the light source, material, current position, ray direction and normal vector at position
                Color p = light_loc.shade(&s,position,-r.get_direction() , s.normal_at(position));
                canvas.write_pixel(row,col,p);
            }
        }
    }
    canvas.save_ppm("ShadedSphere");
}

TEST(GenImage, Spheres){
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

TEST(GenImage, FresnelEffect){
    PointSource ps(WHITE,Tuple({-100,0,0}, TupType::POINT));
    Material base_mat;
    base_mat.set_reflectance(1);
    base_mat.set_pattern(std::make_unique<Checkers>(Checkers(RED, BLUE)));
    Plane base(MatTranslation(0,-1,0), base_mat);
    Material water_mat;
    water_mat.set_reflectance(0);
    water_mat.set_transparency(0.5);
    water_mat.set_refractive_index(WATER);
    Plane water(MatIdentity(4), water_mat);
    Material background_mat;
    background_mat.set_pattern(std::make_unique<Checkers>(Checkers(WHITE, BLACK)));
    Plane back(MatTranslation(100,0,0)*MatRotateZ(pi/2.0));
    World w;
    w.add_shape(std::make_unique<Plane>(std::move(base)));
    w.add_shape(std::make_unique<Plane>(std::move(water)));
    w.add_shape(std::make_unique<Plane>(std::move(back)));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    Tuple from({-10,1,0}, TupType::POINT);
    Tuple to({0,1,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);
    
    auto img = cam.render(&w);
    img->save_ppm("Fresnel");
}

TEST(GenImage,AirBubble){
    PointSource ps(WHITE,Tuple({-10,10,-10}, TupType::POINT));
    Material glass;
    glass.set_color(GRAY);
    glass.set_reflectance(1);
    glass.set_transparency(1);
    glass.set_refractive_index(GLASS);
    Sphere outer(MatScaling(2,2,2), glass);
    Material air;
    air.set_color(WHITE);
    air.set_reflectance(0);
    air.set_transparency(1);
    air.set_refractive_index(AIR);
    Sphere middle(MatIdentity(4), air);
    Material wall_mat;
    wall_mat.set_color(GREEN);
    air.set_reflectance(0);
    air.set_transparency(0);
    air.set_cast_shadow(false);
    auto pat =  Checkers(WHITE,RED);
    wall_mat.set_pattern(std::make_unique<Checkers>(pat));
    
    std::vector<Matrix> operations{MatRotateZ(pi/4.0),MatTranslation(7,0,0)};
    Plane wall(Chain(operations),wall_mat);
    Sphere ref(MatTranslation(0,5,0), glass);
    World w;
    w.add_shape(std::make_unique<Sphere>(std::move(outer)));
    w.add_shape(std::make_unique<Sphere>(std::move(middle)));
    w.add_shape(std::make_unique<Plane>(std::move(wall)));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    Tuple from({-5,0,0}, TupType::POINT);
    Tuple to({0,0,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);
    
    auto img = cam.render(&w);
    img->save_ppm("NestedGlass");
}

TEST(GenImage, SimpleTeapot){
// Assumes that tests are run in build folder which is parallel to obj file
    std::string teapot_obj = "../obj/teapot.obj";
    Parser p;
    p.read(teapot_obj,true);
    auto group = p.emit();
    World w;
    w.add_shape(std::move(group));
    PointSource ps(WHITE,Tuple({-10,10,-10}, TupType::POINT));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    Tuple from({-4.5,4.5,0}, TupType::POINT);
    Tuple to({0,0,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(10,10,pi/2.0, from, to, up);    
    auto img = cam.render(&w);
    img->save_ppm("SimpleTeapot");
}

std::vector<std::tuple<double,double,double,double>> place_random_spheres(int total_spheres=10){
    assert(total_spheres>0);
    RNG rng;
    std::vector<std::tuple<double,double,double,double>> output;
    while(output.size() < total_spheres){
    // generate random translation and size of sphere
        double x = rng.roll_uniform(0,5);
//        double y = rng.roll_uniform(0,5);
        double z = rng.roll_uniform(0,5);
        double rad = rng.roll_normal(0,1);
        double y = rad;
        bool intersected = false;
        // run through all existing spheres
        for(auto const& data: output){
            double dx = std::get<0>(data)-x;
            double dy = std::get<1>(data)-y;
            double dz = std::get<2>(data)-z;
            double min_sep = std::get<3>(data)+rad; // spheres need to have centers be seperated by at least the sum of the radii
            double distance = std::sqrt(dx*dx+dy*dy+dz*dz); // this is the distance between centers of spheres
            if(distance < min_sep){ // if we don't satisfy the min separation, break out and try again
                intersected = true;
                break;
            }
        }
        if(!intersected){ // not intersection, add data
            output.push_back(std::make_tuple(x,y,z,rad));
        }
    }
    return output;
}

TEST(GenImage, RandomSpheres){
    RNG rng;
    World w;
    Material m = Material();
    Plane floor = Plane(MatScaling(10,0.01,10), m);
    w.add_shape(std::make_unique<Plane>(std::move(floor)));
    PointSource ps(WHITE, Tuple({-30,30,-30}, TupType::POINT));
    w.add_source(std::make_unique<PointSource>(std::move(ps)));
    auto sphere_dat = place_random_spheres(50);
    for(auto const& data: sphere_dat){
        Color rc = random_color();
        Material shiny = Material(rc);
        shiny.set_reflectance(1);
        shiny.set_transparency(1);
        shiny.set_refractive_index(GLASS);
        Matrix translation = MatTranslation(std::get<0>(data),std::get<1>(data),std::get<2>(data));
        double rad = std::get<3>(data);
        Matrix scaling = MatScaling(rad,rad,rad);
        Matrix transformation = Chain({scaling, translation});
        w.add_shape(std::make_unique<Sphere>(Sphere(transformation,shiny))); 
    }
    Tuple from({-4,2,5}, TupType::POINT);
    Tuple to({0,1,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);
    auto img = cam.render(&w);
    img->save_ppm("RandomSpheres");
}