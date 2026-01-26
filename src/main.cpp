#include <iostream>
#include "OBJParser.h"
#include "World.h"
#include "PointSource.h"
#include "Sphere.h"
#include "Camera.h"
#include "tools/cpp/runfiles/runfiles.h"
using bazel::tools::cpp::runfiles::Runfiles;

int main(int argc, char** argv){
// std::string error;
//     // Use Create(argv[0]) instead of CreateForTest()
//     auto runfiles = std::unique_ptr<Runfiles>(Runfiles::Create(argv[0], &error));
    
//     if (!runfiles) {
//         // Fallback: If argv[0] fails, try Create() without args
//         runfiles = std::unique_ptr<Runfiles>(Runfiles::Create("", &error));
//     }

//     if (!runfiles) {
//         std::cerr << "Could not initialize runfiles: " << error << std::endl;
//         return 1;
//     }
    
//     // Now Rlocation will work
//     std::string path = runfiles->Rlocation("ray_tracer/obj/Apple.obj");
//     Parser p;
//     p.read(path,true);
//     auto group = p.emit();
// //    group->set_transform(MatScaling(1,1,1.5));
//     World w;
//     w.add_shape(std::move(group));
//     w.add_source(std::make_unique<PointSource>(WHITE,Tuple({-10,10,-10}, TupType::POINT)));
//     Tuple from({-4,5,0}, TupType::POINT);
//     Tuple to({0,0,0}, TupType::POINT);
//     Tuple up({0,1,0});
//     Camera cam(400,400,pi/2.0, from, to, up);    
//     auto img = cam.render(&w);
//     img->save_ppm("Apple");
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
    Tuple up = Tuple({0,1,0});
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
    return 0;
}
