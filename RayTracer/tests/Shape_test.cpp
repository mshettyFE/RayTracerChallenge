#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Constants.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Canvas.h"
#include <memory>

TEST(ShapeTest, UniqueID){
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Plane>(Plane()));
    shapes.push_back(std::make_shared<Plane>(Plane()));
    shapes.push_back(std::make_shared<Plane>(Plane()));
    shapes.push_back(std::make_shared<Sphere>(Sphere()));
    shapes.push_back(std::make_shared<Sphere>(Sphere()));
    shapes.push_back(std::make_shared<Sphere>(Sphere()));
    EXPECT_NE(shapes[0]->get_id() , shapes[1]->get_id());
    EXPECT_NE(shapes[3]->get_id() , shapes[0]->get_id());
}