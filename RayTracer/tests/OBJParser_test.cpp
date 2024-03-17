#include <gtest/gtest.h>
#include <iostream>
#include "OBJParser.h"

TEST(OBJParser, Gibberish){
    Parser p;
    p.read("There was a young lady named Bright\n"
"who traveled much faster than light.\n"
"She set out one day\n"
"in a relative way,\n"
"and came back the previous night.\n", false);
    EXPECT_EQ(p.get_invalid_reads(),5);
}

TEST(OBJParser, VertexParser){
    Parser p;
    p.read("v -1 1 0\n"
"v -1.0000 0.5000 0.0000\n"
"v 1 0 0\n"
"v 1 1 0\n", false);
    EXPECT_EQ(p.get_invalid_reads(),0);
    EXPECT_THROW({
        try{
            p.get_vertex(0);
        }
        catch(const std::invalid_argument &e){
            EXPECT_STREQ( "0 Out of bounds on parser for vertex (0 is out of bounds)", e.what() );
            throw;
        }
    }
    ,std::invalid_argument);
    EXPECT_EQ(p.get_vertex(1),GenPoint(-1,1,0));
    EXPECT_EQ(p.get_vertex(2),GenPoint(-1,0.5,0));
    EXPECT_EQ(p.get_vertex(3),GenPoint(1,0,0));
    EXPECT_EQ(p.get_vertex(4),GenPoint(1,1,0));
}

TEST(OBJParser, ReadTriangle){
    Parser p;
    p.read("v -1 1 0\n"
"v -1 0 0\n"
"v 1 0 0\n"
"v 1 1 0\n"
"v 0 2 0\n"
"f 1 2 3 4 5\n",false);
    EXPECT_EQ(p.get_total_vertices(), 6);
    auto vertices = p.get_vertices();
    auto g = p.emit();
    const Shape* first_group = g->get_children()[0].get();
    EXPECT_EQ(first_group->get_total_children(), 3);
    const std::vector<std::unique_ptr<Shape>>& children = first_group->get_children();
    auto  points_triangle_one = dynamic_cast<const Triangle*>(children[0].get())->get_points();
    EXPECT_EQ(points_triangle_one[0],vertices[1]);
    EXPECT_EQ(points_triangle_one[1],vertices[2]);
    EXPECT_EQ(points_triangle_one[2],vertices[3]);
    auto  points_triangle_two = dynamic_cast<const Triangle*>(children[1].get())->get_points();
    EXPECT_EQ(points_triangle_two[0],vertices[1]);
    EXPECT_EQ(points_triangle_two[1],vertices[3]);
    EXPECT_EQ(points_triangle_two[2],vertices[4]);
    auto  points_triangle_three = dynamic_cast<const Triangle*>(children[2].get())->get_points();
    EXPECT_EQ(points_triangle_three[0],vertices[1]);
    EXPECT_EQ(points_triangle_three[1],vertices[4]);
    EXPECT_EQ(points_triangle_three[2],vertices[5]);
}

TEST(OBJParser, NamedGroups){
    Parser p;
    p.read(
"v -1 1 0\n"
"v -1 0 0\n"
"v 1 0 0\n"
"v 1 1 0\n"
"g FirstGroup\n"
"f 1 2 3\n"
"g SecondGroup\n"
"f 1 3 4\n",false
    );
    auto vertices = p.get_vertices();
    auto g = p.emit();   
    const Shape* first_group = g->get_children()[0].get();
    const Shape* second_group = g->get_children()[1].get();
    EXPECT_EQ(g->get_total_children(), 2);
    const std::vector<std::unique_ptr<Shape>>& children = first_group->get_children();
    EXPECT_EQ(children.size(),1);
    auto  points_triangle_one = dynamic_cast<const Triangle*>(first_group->get_children()[0].get())->get_points();
    EXPECT_EQ(points_triangle_one[0],vertices[1]);
    EXPECT_EQ(points_triangle_one[1],vertices[2]);
    EXPECT_EQ(points_triangle_one[2],vertices[3]);
    const std::vector<std::unique_ptr<Shape>>& children2 = second_group->get_children();
    EXPECT_EQ(children2.size(),1);
    auto  points_triangle_two = dynamic_cast<const Triangle*>(second_group->get_children()[0].get())->get_points();
    EXPECT_EQ(points_triangle_two[0],vertices[1]);
    EXPECT_EQ(points_triangle_two[1],vertices[3]);
    EXPECT_EQ(points_triangle_two[2],vertices[4]);
}

TEST(OBJParser, ParseNormalVertices){
    Parser p;
    p.read("vn 0 0 1\n"
"vn 0.707 0 -0.707\n"
"vn 1 2 3\n",false);
    auto  normals = p.get_normals();
    EXPECT_EQ(normals.size(), 4);
    EXPECT_EQ(normals[0],GenVec(0,0,0));
    EXPECT_EQ(normals[1], GenVec(0,0,1));
    EXPECT_EQ(normals[2], GenVec(0.707, 0, -0.707));
    EXPECT_EQ(normals[3], GenVec(1, 2, 3));
}


TEST(OBJParser, FacesWithNormals){
    Parser p;
    p.read("v 0 1 0\n"
"v -1 0 0\n"
"v 1 0 0\n"
"vn -1 0 0\n"
"vn 1 0 0\n"
"vn 0 1 0\n"
"f 1//3 2//1 3//2\n"
"f 1/0/3 2/102/1 3/14/2\n", false);
}