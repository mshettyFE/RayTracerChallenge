#include <gtest/gtest.h>
#include <iostream>
#include "Constants.h"
#include <memory>
#include "Shape.h"
#include "Sphere.h"
#include "Impact.h"

TEST(ImpactTest, InitImpact){
    auto shp = std::make_shared<Sphere>(Sphere());
    Impact imp(4.0,shp);
    EXPECT_EQ(imp.get_t(), 4.0);
    EXPECT_EQ(imp.get_obj(), shp);
}