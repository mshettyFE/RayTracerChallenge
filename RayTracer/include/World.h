#ifndef WORLD_H
#define WORLD_H

#include "Shape.h"
#include "Ray.h"
#include "BVH.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Color.h"
#include "LightSource.h"
#include <memory>
#include <vector>

class World{
    private:
        std::vector<std::unique_ptr<LightSource>> sources;
        std::vector<std::unique_ptr<Shape>> shapes;
        std::unique_ptr<BVH> bvh=nullptr;
    public:

    void init_bvh(){
        bvh = std::make_unique<BVH>(BVH(shapes));
    }
    
    std::vector<Impact> intersect(const Ray& r) const;

    Color  shade_hit(const CollisionInfo& hit, unsigned int remaining=5) const;

    Color color_at(const Ray& r, unsigned int remaining=5) const;

    int number_of_sources() const;
    int number_of_shapes() const;

    const Shape* get_shape(int i) const ;
    const LightSource* get_source(int i) const;

    void set_shape(int obj, std::unique_ptr<Shape>& other) ;
    void set_source(int obj, std::unique_ptr<LightSource>& other) ;

    void add_shape(std::unique_ptr<Shape> new_shape);
    void add_source(std::unique_ptr<LightSource> new_source);

    bool is_shadowed(const Tuple& pt) const;

    Color reflect_color(const CollisionInfo& comps, unsigned int remaining=5) const;

    Color refract_color(const CollisionInfo& comps, unsigned int remaining=5) const;

    friend std::ostream& operator <<(std::ostream& out, const World& w);

};

std::unique_ptr<World> default_world();

#endif