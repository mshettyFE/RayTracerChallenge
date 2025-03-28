#include "World.h"
#include "Shape.h"
#include "Sphere.h"
#include "Color.h"
#include "Tuple.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Ray.h"
#include "PointSource.h"
#include "Camera.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <stdexcept>

int World::number_of_sources() const{
    return sources.size();
}
int World::number_of_shapes() const{
    return shapes.size();
}

std::vector<Impact> World::intersect(const Ray& r, bool use_bvh) const{
    std::vector<Impact> all_hits;
    if(use_bvh){
        all_hits = bvh->intersect(r);
    }
    else{
        for(auto const& shape: shapes){
            for(auto hit: shape->intersect(r)){
                all_hits.push_back(hit);
            }
        }    
        std::sort(all_hits.begin(), all_hits.end(), [](const Impact& a, const Impact& b) -> bool{ return (a.get_t() < b.get_t());  });
    }
    return all_hits;
}

Color  World::shade_hit(const CollisionInfo& hit, unsigned int remaining) const{
    Color total = BLACK;
    bool shadowed = is_shadowed(hit.get_over_pnt());
    double mat_ref = hit.get_impact().get_obj()->get_material().get_reflectance();
    double mat_transp = hit.get_impact().get_obj()->get_material().get_transparency();
    for(auto const& source: sources){
        Color surface =  source->shade(hit.get_impact().get_obj(), hit.get_pnt(),hit.get_eye(), hit.get_normal(), shadowed );
        Color reflected_color = reflect_color(hit, remaining);
        Color refracted_color = refract_color(hit,remaining);
        if((mat_ref > 0) && (mat_transp >0)){
            mat_ref = hit.schlick();
            total += surface+ reflected_color*mat_ref+refracted_color*(1.0-mat_ref);
        }
        else{
            total += surface+ reflected_color+refracted_color;
        }
    }
    return total;
}

const Shape* World::get_shape(int i) const {
    if(i<0){
        throw std::invalid_argument("Index is negative");
    }
    if(i >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    return this->shapes[i].get();
}


const LightSource* World::get_source(int i) const {
    if(i<0){
        throw std::invalid_argument("Index is negative");
    }
    if(i >= this->number_of_sources()){
        throw std::invalid_argument("Index is out of bounds");
    }
    return this->sources[i].get();
}

void World::set_shape(int obj, std::unique_ptr<Shape>& other)  {
    if(obj<0){
        throw std::invalid_argument("Index is negative");
    }
    if(obj >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    this->shapes[obj] =std::move( other);
}

void World::set_source(int obj, std::unique_ptr<LightSource>& other) {
    if(obj<0){
        throw std::invalid_argument("Index is negative");
    }
    if(obj >= this->number_of_shapes()){
        throw std::invalid_argument("Index is out of bounds");
    }
    this->sources[obj] = std::move(other);
}


Color World::color_at(const Ray& r, unsigned int remaining) const{
    Camera::total_rays += 1;
    std::vector<Impact> hits = intersect(r);
    Color out = BLACK;
    if (hits.size() > 0){
        int lowest_positive_index=-1;
        for(unsigned int i=0; i<hits.size(); ++i){
            if(hits[i].get_t() >= 0){
                lowest_positive_index = i;
                break;
            }
        }
        // If all of the hits are negative, there is nothing in front of the array, so return BLACK
        if(lowest_positive_index==-1){
            return out;
        }
        out += shade_hit(CollisionInfo(std::make_unique<Impact>(hits[lowest_positive_index]),r), remaining);
    }
    return out;
}

std::unique_ptr<World> default_world(){
    Material mat(0.1,0.7,0.2,200.0,Color({0.8,1.0,0.6}));
    std::unique_ptr<Sphere> s1 = std::make_unique<Sphere>(MatIdentity(4),mat);
    std::unique_ptr<Sphere> s2 = std::make_unique<Sphere>(MatScaling(0.5,0.5,0.5));
//    std::vector<std::unique_ptr<Shape>> shapes;
//    shapes.push_back(std::move(s1));
//    shapes.push_back(std::move(s2));
    std::unique_ptr<PointSource> source = std::make_unique<PointSource>(Color(1,1,1), Tuple({-10,10,-10}, TupType::POINT));
//    std::vector<std::unique_ptr<LightSource>> sources;
//    sources.push_back(std::move(source));
    auto w = std::make_unique<World>();
    w->add_shape(std::move(s1));
    w->add_shape(std::move(s2));
    w->add_source(std::move(source));
    w->init_bvh();
    return w;
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
    for(auto const& source: sources){
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

void World::add_shape(std::unique_ptr<Shape> new_shape){
    shapes.push_back(std::move(new_shape));
};

void World::add_source(std::unique_ptr<LightSource> new_source){
    sources.push_back(std::move(new_source));
}


Color World::reflect_color(const CollisionInfo&  comps, unsigned int remaining) const{
    if(remaining <= 0){
        return BLACK;
    }
    if(comps.get_impact().get_obj()->get_material().get_reflectance() < glob_resolution){
        return BLACK;
    }
    Ray r = Ray(comps.get_over_pnt(), comps.get_reflect());
    Camera::total_rays += 1;
    Color col = this->color_at(r, remaining-1);
    return col*comps.get_impact().get_obj()->get_material().get_reflectance();
}

Color World::refract_color(const CollisionInfo& comps, unsigned int remaining) const{
    if(remaining <=0){
        return BLACK;
    }
    if(comps.get_impact().get_obj()->get_material().get_transparency() == 0){
        return BLACK;
    }
    double n_ratio = comps.get_n1()/comps.get_n2();
    double cos_i = comps.get_eye().dot(comps.get_normal());
    double sin2_theta = n_ratio*n_ratio*(1-cos_i*cos_i);
    if(sin2_theta > 1){
        return BLACK;
    }
    Camera::total_rays += 1;
    double cos_t = std::sqrt(1.0-sin2_theta);
    Tuple new_direction = comps.get_normal()*(n_ratio*cos_i-cos_t)- comps.get_eye()*n_ratio;
    Ray refracted_ray(comps.get_under_pnt(), new_direction);
    return color_at(refracted_ray,remaining-1)*comps.get_impact().get_obj()->get_material().get_transparency();
}