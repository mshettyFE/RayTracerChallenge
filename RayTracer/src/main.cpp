#include <iostream>
#include "OBJParser.h"
#include "World.h"
#include "PointSource.h"
#include "Camera.h"


int main(){
// Assumes that tests are run in build folder which is parallel to obj file
    std::string teapot_obj = "../obj/teapot.obj";
    Parser p;
    p.read(teapot_obj,true);
    auto group = p.emit();
    group->set_transform(Chain({MatRotateY(pi),MatRotateX(pi/2)}));
    World w;
    w.add_shape(std::move(group));
    w.add_source(std::make_unique<PointSource>( WHITE,Tuple({-10,10,-10}, TupType::POINT))   );
    Tuple from({-4.5,4.5,0}, TupType::POINT);
    Tuple to({0,0,0}, TupType::POINT);
    Tuple up({0,0,-1});
    Camera cam(100,100,pi/2.0, from, to, up);    
    auto img = cam.render(&w);
    img->save_ppm("SimpleTeapot");
  return 0;
}
