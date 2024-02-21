#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "PointSource.h"
#include "Shape.h"
#include "Ray.h"
#include "Intersection.h"
#include <memory>

class World{
private:
    std::vector<std::shared_ptr<PointSource>> sources;
    std::vector<std::shared_ptr<Shape>> shapes;
public:

    World(std::vector<std::shared_ptr<PointSource>> light_sources, std::vector<std::shared_ptr<Shape>> all_shapes);
    World(std::shared_ptr<PointSource> light_sources, std::shared_ptr<Shape>  shape);

    std::vector<Intersection> intersect(const Ray& r);


    int number_of_sources() const;
    int number_of_shapes() const;
};

World default_world();


#endif