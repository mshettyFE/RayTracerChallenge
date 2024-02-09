#ifndef CANVAS_H
#define CANVAS_H

#include <array>
#include <string>
#include "Color.h"

class Canvas{
    protected:
        unsigned int width;
        unsigned int height;
        unsigned int n_pixels;
        std::vector<Color> pixels;
    public:
        Canvas(unsigned int a_width, unsigned int a_height);
        Canvas(std::string fname);
        Color operator()(unsigned int cand_w, unsigned int cand_h);
        int get_index(unsigned int x, unsigned int y);
        void write_pixel(unsigned int x, unsigned int y, Color c);
        void save_ppm(std::string fname);
        void print();
};


#endif