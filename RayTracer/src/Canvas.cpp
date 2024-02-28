#include <array>
#include <stdexcept>
#include <fstream>
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
    for(int i=0; i<this->n_pixels(); ++i){
        pixels.push_back(Color(0,0,0));
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

int Canvas::n_pixels() const{
    return width*height;
};


int Canvas::get_index(unsigned int cand_w, unsigned int cand_h) const{
    if (cand_w>= width){
        throw std::invalid_argument("Invalid Width");
    }
    if (cand_h>= height){
        throw std::invalid_argument("Invalid Height");
    }
    return cand_w*height+cand_h;
}


void Canvas::write_pixel(unsigned int cand_w, unsigned int cand_h, const Color& c){
    int i  = get_index(cand_w, cand_h);
    pixels[i] = c;
}

std::ostream& operator << (std::ostream &out, const Canvas& other){
    for(int i=0; i< other.width; ++i){
        for(int j=0; j<other.height; ++j){
            out << "Row: " << i << '\t' << "Col: " << j << '\t' << "Index:" << '\t' << other.get_index(i,j) << '\t'<<  other.pixels[other.get_index(i,j)]  << std::endl;
        }
    }
    return out;
}

void Canvas::save_ppm(std::string fname){
    std::string out_fname =  fname+".ppm";
    std::cout << "Saving " << out_fname << std::endl;
    std::ofstream output_file (out_fname);
    std::string cur_line;
    if (output_file.is_open()){
        // specific formate of P3
        output_file << "P3\n";
        // specify width/height of image
        output_file << width << " " << height << '\n';
        // specify max value of image
        output_file << 255 << "\n";
        for(int i=0; i<this->n_pixels(); ++i){
            // loop to write all pixels to .ppm file
            Color cur_pixel = pixels[i];
            // rescale values to lie within 0-255
            cur_pixel.rescale();
            for(int color=0; color< cur_pixel.get_dim(); ++color){
                std::string cur_num_string = std::to_string((int) cur_pixel[color]);
                cur_num_string.append(" ");
                // ppm standard has hard cap of 70 characters per line. This checks if adding a new color will exceed that limit.
                // If so, write line to file, and then add new color to a new line
                if (cur_line.length()+cur_num_string.length()>70){
                    output_file << cur_line << "\n";
                    cur_line.clear();
                    cur_line.append(cur_num_string);
                }
                else{
                    cur_line.append(cur_num_string);
                }
            }
        }
        // flush last line to file
        output_file << cur_line << '\n';
        output_file.close();
    }
}
