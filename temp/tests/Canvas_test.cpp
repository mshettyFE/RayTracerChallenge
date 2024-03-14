#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "Canvas.h"
#include "Constants.h"

TEST(CanvasTest, CanvasBasics){
    Canvas c(2,3);
    Color test(0,0,0);
    for(int i=0; i<2; ++i){
        for(int j=0; j<3; ++j){
            EXPECT_EQ(test,c(i,j));
        }
    }
}

TEST(CanvasTest, WritePixel){
    Canvas c(2,3);
    c.write_pixel(0,0,RED);
    Color a = c(0,0);
    EXPECT_EQ(a,RED);
}