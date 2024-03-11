#include "PointSource.h"
#include "LightSource.h"
#include "World.h"
#include "Camera.h"
#include "Canvas.h"
#include "Group.h"
#include "Sphere.h"
#include <gtest/gtest.h>

TEST(GroupTest,NewChild){
    Group g;
    EXPECT_EQ(g.get_transform(),MatIdentity(4));
    EXPECT_EQ(g.get_total_children(), 0);
    Sphere s;
    g.add_child<Sphere>(s);
    EXPECT_EQ(s.get_parent(), g.get_group_pointer());
}

TEST(GroupTest, Printing){
    Group g1;
    Sphere s1;
    g1.add_child<Sphere>(s1);
    Sphere s2;
    g1.add_child<Sphere>(s2);
    Group g2;
    Sphere s3;
    g2.add_child<Sphere>(s3);
    Sphere s4;
    g2.add_child<Sphere>(s4);
    g1.add_child<Group>(g2);
    g1.verbose_print();
}