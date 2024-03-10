#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "PointSource.h"
#include "Shape.h"
#include "Ray.h"
#include "Impact.h"
#include "CollisionInfo.h"
#include "Color.h"
#include <memory>

class World{
private:
    std::vector<std::shared_ptr<LightSource>> sources;
    std::vector<std::shared_ptr<Shape>> shapes;
public:

    World(std::vector<std::shared_ptr<LightSource>> light_sources, std::vector<std::shared_ptr<Shape>> all_shapes);
    World(std::shared_ptr<LightSource> light_sources, std::shared_ptr<Shape>  shape);

    std::vector<Impact> intersect(const Ray& r) const;

    Color  shade_hit(const CollisionInfo& hit, unsigned int remaining=5);

    Color color_at(const Ray& r, unsigned int remaining=5);

    int number_of_sources() const;
    int number_of_shapes() const;

    std::shared_ptr<Shape> get_shape(int i) const ;
    std::shared_ptr<LightSource> get_source(int i) const;

    void set_shape(int i, std::shared_ptr<Shape> other);

    void add_shape(std::shared_ptr<Shape> new_shape);

    bool is_shadowed(const Tuple& pt) const;

    Color reflect_color(const CollisionInfo& comps, unsigned int remaining=5);

    Color refract_color(const CollisionInfo& comps, unsigned int remaining=5);

    friend std::ostream& operator <<(std::ostream& out, const World& w);

};

World default_world();


#endif