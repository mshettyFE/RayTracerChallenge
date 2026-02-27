#include <iostream>
#include "OBJParser.h"
#include "World.h"
#include "PointSource.h"
#include "Sphere.h"
#include "Camera.h"
#include "tools/cpp/runfiles/runfiles.h"
using bazel::tools::cpp::runfiles::Runfiles;

int main(int argc, char** argv){
std::string error;
    // Use Create(argv[0]) instead of CreateForTest()
    auto runfiles = std::unique_ptr<Runfiles>(Runfiles::Create(argv[0], &error));
    
    if (!runfiles) {
        // Fallback: If argv[0] fails, try Create() without args
        runfiles = std::unique_ptr<Runfiles>(Runfiles::Create("", &error));
    }

    if (!runfiles) {
        std::cerr << "Could not initialize runfiles: " << error << std::endl;
        return 1;
    }
    
    // Now Rlocation will work
    std::string path = runfiles->Rlocation("ray_tracer/obj/Apple.obj");
    Parser p;
    p.read(path,true);
    auto group = p.emit();
//    group->set_transform(MatScaling(1,1,1.5));
    World w;
    w.add_shape(std::move(group));
    w.add_source(std::make_unique<PointSource>(WHITE,Tuple({-10,10,-10}, TupType::POINT)));
    Tuple from({-4,5,0}, TupType::POINT);
    Tuple to({0,0,0}, TupType::POINT);
    Tuple up({0,1,0});
    Camera cam(400,400,pi/2.0, from, to, up);    
    auto img = cam.render(&w);
    img->save_ppm("Apple");
    return 0;
}
