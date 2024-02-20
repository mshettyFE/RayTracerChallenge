#include "World.h"
#include "Sphere.h"
#include "Color.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Ray.h"
#include <algorithm>
#include <vector>
#include <memory>

World::World(std::vector<std::shared_ptr<PointSource>> light_sources, std::vector<std::shared_ptr<Shape>> all_shapes){
    sources = light_sources;
    shapes = all_shapes;
}

World::World(std::shared_ptr<PointSource> light_source, std::shared_ptr<Shape>  shape){
    sources.push_back(light_source);
    shapes.push_back(shape);    
}


int World::number_of_sources() const{
    return sources.size();
}
int World::number_of_shapes() const{
    return shapes.size();
}

std::vector<Intersection> World::intersect(const Ray& r){
    std::vector<Intersection> all_hits;
    for(auto shape: shapes){
        for(double hit: shape->intersect(r)){
            all_hits.push_back(Intersection{hit,shape});
        }
    }
    std::sort(all_hits.begin(), all_hits.end(), [](const Intersection& a, const Intersection& b) -> bool{ return a.t < b.t;  });
    return all_hits;
}


World default_world(){
    Material mat(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Sphere(MatIdentity(4),mat));
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Sphere(MatScaling(0.5,0.5,0.5)));
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(s1);
    shapes.push_back(s2);
    std::shared_ptr<PointSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,10}, TupType::POINT)));
    std::vector<std::shared_ptr<PointSource>> sources;
    sources.push_back(source);
    return World(sources, shapes);
}
