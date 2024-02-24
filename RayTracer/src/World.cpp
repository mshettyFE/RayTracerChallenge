#include "World.h"
#include "Sphere.h"
#include "Color.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Ray.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <stdexcept>

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
            all_hits.push_back(Intersection(hit, shape, r));
        }
    }
    
    std::sort(all_hits.begin(), all_hits.end(), [](const Intersection& a, const Intersection& b) -> bool{ return (a.get_t() < b.get_t());  });
    return all_hits;
}

Color  World::shade_hit(const Intersection& hit){
    Color c = BLACK;
    for(auto source: sources){
        c += source->shade(hit.get_obj()->get_material(),hit.get_pnt(),hit.get_eye(), hit.get_normal() );
    }
    return c;
}

std::shared_ptr<Shape> World::get_shape(int i) const{
    if(i<0){
        throw std::invalid_argument("Index is negative");
    }
    if(i >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    return this->shapes[i];
}

std::shared_ptr<PointSource> World::get_source(int i) const {
    if(i<0){
        throw std::invalid_argument("Index is negative");
    }
    if(i >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    return this->sources[i];
}

Color World::color_at(const Ray& r){
    std::vector<Intersection> hits = intersect(r);
    Color out = BLACK;
    if (hits.size() > 0){
        int lowest_positive_index=-1;
        for(int i=0; i<hits.size(); ++i){
            std::cout << "Index" << i << " " << hits[i] << std::endl;
            if(hits[i].get_t() >= 0){
                lowest_positive_index = i;
                break;
            }
        }
        // If all of the hits are negative, there is nothing in front of the array, so return BLACK
        if(lowest_positive_index==-1){
            return out;
        }
        for(auto source : sources){
            out += shade_hit(hits[lowest_positive_index]);
        }
    }
    return out;
}


World default_world(){
//    Material mat(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Sphere(MatIdentity(4)));
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Sphere(MatScaling(0.5,0.5,0.5)));
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(s1);
    shapes.push_back(s2);
    std::shared_ptr<PointSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT)));
    std::vector<std::shared_ptr<PointSource>> sources;
    sources.push_back(source);
    return World(sources, shapes);
}

std::ostream& operator << (std::ostream &out, const World& w){
    out << "Sources" << std::endl;
    for(int source=0; source<w.number_of_sources(); source++){
        out <<"Source:" << source << "\n\t" <<  *(w.get_source(source)) << std::endl;
    }
    out <<"Shapes" << "\n";
    for(int shape=0; shape<w.number_of_shapes(); shape++){
        out <<"Shape:" << shape << "\n";
        w.get_shape(shape)->print();
    }
    return out;
}
