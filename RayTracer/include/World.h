#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Shape.h"
#include "Ray.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Color.h"
#include <memory>
#include "PointSource.h"

class World{
    private:
        std::vector<std::unique_ptr<LightSource>> sources = {};
        std::vector<std::unique_ptr<Shape>> shapes = {};
    public:

    std::vector<Impact> intersect(const Ray& r) const;

    Color  shade_hit(const CollisionInfo& hit, unsigned int remaining=5);

    Color color_at(const Ray& r, unsigned int remaining=5);

    int number_of_sources() const;
    int number_of_shapes() const;

    const Shape* get_shape(int i) const ;
    const LightSource* get_source(int i) const;

    void set_shape(int obj, std::unique_ptr<Shape>& other) ;
    void set_source(int obj, std::unique_ptr<LightSource>& other) ;

    void add_shape(std::unique_ptr<Shape> new_shape);
    void add_source(std::unique_ptr<LightSource> new_source);

    bool is_shadowed(const Tuple& pt) const;

    Color reflect_color(const CollisionInfo& comps, unsigned int remaining=5);

    Color refract_color(const CollisionInfo& comps, unsigned int remaining=5);

    friend std::ostream& operator <<(std::ostream& out, const World& w);

};

std::unique_ptr<World> default_world();

#endif