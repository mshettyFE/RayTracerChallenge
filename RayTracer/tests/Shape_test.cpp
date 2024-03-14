#include "Constants.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include <memory>
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>

TEST(ShapeTest, UniqueID){
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Plane>(Plane()));
    shapes.push_back(std::make_unique<Plane>(Plane()));
    shapes.push_back(std::make_unique<Plane>(Plane()));
    shapes.push_back(std::make_unique<Sphere>(Sphere()));
    shapes.push_back(std::make_unique<Sphere>(Sphere()));
    shapes.push_back(std::make_unique<Sphere>(Sphere()));
    EXPECT_NE(shapes[0]->get_id() , shapes[1]->get_id());
    EXPECT_NE(shapes[3]->get_id() , shapes[0]->get_id());
}