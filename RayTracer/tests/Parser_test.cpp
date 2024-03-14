#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "OBJParser.h"

TEST(OBJParserTest, Gibberish){
    Parser p;
    p.read("There was a young lady named Bright\n"
"who traveled much faster than light\n"
"She set out one day\n"
"in a relative way,\n"
"and came back the previous night.\n", false);
    EXPECT_EQ(p.get_invalid_reads(),5);
}

TEST(OBJParserTest, Vertices){
    Parser p;
    p.read("v -1 1 0\n"
"v -1.0000 0.5000 0.0000\n"
"v 1 0 0\n"
"v 1 1 0\n", false);
    EXPECT_EQ(p.get_invalid_reads(),0);
    EXPECT_EQ(p.get_total_vertices(),5);
    EXPECT_THROW({
        try{            p.get_vertex(0);}
        catch(const std::invalid_argument &e){            EXPECT_STREQ( "Out of bounds on parser (0 is out of bounds)", e.what() ); throw;}
    }
    ,std::invalid_argument);
    EXPECT_EQ(p.get_vertex(1),GenPoint(-1,1,0));
    EXPECT_EQ(p.get_vertex(2),GenPoint(-1,0.5,0));
    EXPECT_EQ(p.get_vertex(3),GenPoint(1,0,0));
    EXPECT_EQ(p.get_vertex(4),GenPoint(1,1,0));
}
