#include <array>
#include <stdexcept>
#include "Canvas.h"

Canvas::Canvas(unsigned int a_width, unsigned int a_height){
    if(a_width == 0){
        throw std::invalid_argument("Canvas Width must be greater than 0");
    }
    if(a_height == 0){
        throw std::invalid_argument("Canvas Height must be greater than 0");
    }
    width = a_width;
    height = a_height;
    n_pixels = width*height;
    pixels.reserve(n_pixels);
    for(int i=0; i<n_pixels; ++i){
        pixels[i] = Color(0,0,0);
    }
}

Color Canvas::operator()(unsigned int cand_w, unsigned int cand_h){
    if (cand_w>= width){
        throw std::invalid_argument("Invalid Width");
    }
    if (cand_h>= height){
        throw std::invalid_argument("Invalid Height");
    }
    unsigned int index = cand_h*width+cand_w;
    Color output = pixels[index];
    return output;
}

int Canvas::get_index(unsigned int cand_w, unsigned int cand_h){
    if (cand_w>= width){
        throw std::invalid_argument("Invalid Width");
    }
    if (cand_h>= height){
        throw std::invalid_argument("Invalid Height");
    }
    return cand_h*width+cand_w;
}


void Canvas::write_pixel(unsigned int cand_w, unsigned int cand_h, Color c){
    int i  =get_index(cand_w, cand_h);
    std::cout << i << std::endl;
    pixels[i] = c;
}

void Canvas::print(){
    for(int i=0; i< width; ++i){
        for(int j=0; j<height; ++j){
            std::cout << get_index(i,j) << std::endl;
            std::cout << "Row: " << i << '\t' << "Col: " << j << '\t' << pixels[get_index(i,j)]  << std::endl;
        }
    }
}
