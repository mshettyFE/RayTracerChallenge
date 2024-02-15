#ifndef CANVAS_H
#define CANVAS_H

#include <array>
#include <string>
#include "Color.h"

class Canvas{
    protected:
        //dimensions of canvas
        unsigned int width;
        unsigned int height;
        // Actual RGB values of pixels
        std::vector<Color> pixels;
    public:
        // Constructor
        Canvas(unsigned int a_width, unsigned int a_height);
        // pixel access operator
        Color operator()(unsigned int cand_w, unsigned int cand_h);
        // convenience function for number of pixels
        int n_pixels() const;
        // Convenience function to map (x,y) tuple to single index to select a pixel from pixels vector
        int get_index(unsigned int x, unsigned int y) const;
        // write a pixel to the canvas with color C
        void write_pixel(unsigned int x, unsigned int y, const Color& c);
        // output current state of canvas to ppm format
        void save_ppm(std::string fname);

        // print state of canvas
        friend std::ostream& operator << (std::ostream &out, const Canvas& other);
};


#endif