#include "World.h"
#include "Shape.h"
#include "Sphere.h"
#include "Color.h"
#include "Tuple.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Ray.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <stdexcept>

World::World(std::vector<std::shared_ptr<LightSource>> light_sources, std::vector<std::shared_ptr<Shape>> all_shapes){
    sources = light_sources;
    shapes = all_shapes;
}

World::World(std::shared_ptr<LightSource> light_source, std::shared_ptr<Shape>  shape){
    sources.push_back(light_source);
    shapes.push_back(shape);    
}


int World::number_of_sources() const{
    return sources.size();
}
int World::number_of_shapes() const{
    return shapes.size();
}

std::vector<Impact> World::intersect(const Ray& r) const{
    std::vector<Impact> all_hits;
    for(auto shape: shapes){
        for(double hit: shape->intersect(r)){
            all_hits.push_back(Impact(hit, shape));
        }
    }
    
    std::sort(all_hits.begin(), all_hits.end(), [](const Impact& a, const Impact& b) -> bool{ return (a.get_t() < b.get_t());  });
    return all_hits;
}

Color  World::shade_hit(const CollisionInfo& hit, unsigned int remaining){
    Color c = BLACK;
    bool shadowed = is_shadowed(hit.get_over_pnt());
    for(auto source: sources){
        c += source->shade(hit.get_impact().get_obj(),hit.get_pnt(),hit.get_eye(), hit.get_normal(), shadowed );
        c += reflect_color(hit, remaining);
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

std::shared_ptr<LightSource> World::get_source(int i) const {
    if(i<0){
        throw std::invalid_argument("Index is negative");
    }
    if(i >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    return this->sources[i];
}

Color World::color_at(const Ray& r, unsigned int remaining){
    std::vector<Impact> hits = intersect(r);
    Color out = BLACK;
    if (hits.size() > 0){
        int lowest_positive_index=-1;
        for(int i=0; i<hits.size(); ++i){
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
            out += shade_hit(CollisionInfo(hits[lowest_positive_index],r), remaining);
        }
    }
    return out;
}

World default_world(){
    Material mat(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Sphere(MatIdentity(4),mat));
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Sphere(MatScaling(0.5,0.5,0.5)));
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(s1);
    shapes.push_back(s2);
    std::shared_ptr<PointSource> source = std::make_shared<PointSource>(PointSource(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT)));
    std::vector<std::shared_ptr<LightSource>> sources;
    sources.push_back(source);
    return World(sources, shapes);
}

std::ostream& operator << (std::ostream &out, const World& w){
    out << "Sources" << std::endl;
    for(int source=0; source<w.number_of_sources(); source++){
        w.get_source(source)->print();
    }
    out <<"Shapes" << "\n";
    for(int shape=0; shape<w.number_of_shapes(); shape++){
        out <<"Shape:" << shape << "\n";
        w.get_shape(shape)->print();
    }
    return out;
}


bool World::is_shadowed(const Tuple& pt) const{
    for(auto source: sources){
        Tuple v = source->get_position()-pt;
        double distance = v.L2Norm();
        v.normalize();
        Ray ray = Ray(pt,v);
        std::vector<Impact> hits =  intersect(ray);
        Impact first = first_hit(hits);
        if(first.get_obj() == nullptr){
            continue;
        }
        if (first.get_t()*first.get_t() < distance){
            return first.get_obj()->get_material().is_casting_shadow(); // if inbetween object is casting a shadow (for instance, plane of water above rocks shouldnt cast a shadow)
        }
    }
    return false;
}

void World::add_shape(std::shared_ptr<Shape> new_shape){
    shapes.push_back(new_shape);
};

Color World::reflect_color(const CollisionInfo&  comps, unsigned int remaining){
    if(remaining <= 0){
        return BLACK;
    }
    if(comps.get_impact().get_obj()->get_material().get_reflectance() < glob_resolution){
        return BLACK;
    }
    Ray r = Ray(comps.get_over_pnt(), comps.get_reflect());
    Color col = this->color_at(r, remaining-1);
    return col*comps.get_impact().get_obj()->get_material().get_reflectance();
}