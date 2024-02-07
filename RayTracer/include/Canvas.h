#ifndef CANVAS_H
#define CANVAS_H

#include <array>
#include "Color.h"

class Canvas{
    protected:
        unsigned int width;
        unsigned int height;
        unsigned int n_pixels;
        std::vector<Color> pixels;
    public:
        Canvas(unsigned int a_width, unsigned int a_height);
        Color operator()(unsigned int cand_w, unsigned int cand_h);
        int get_index(unsigned int x, unsigned int y);
        void write_pixel(unsigned int x, unsigned int y, Color c);
        void print();
};


#endif