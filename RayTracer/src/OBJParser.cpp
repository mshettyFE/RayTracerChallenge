#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "OBJParser.h"
#include "Triangle.h"
#include "Group.h"

std::unique_ptr<Group> Parser::read(const std::string& fname, bool file){
    invalid_reads = 0;
    current_line = 0;
    if(file){
// for now, just assume that fname is valid filename
        std::ifstream myfile(fname.c_str());
        if (myfile.is_open()){
            std::string line;
            line_item parsed_line;
            while(std::getline(myfile, line)){
                current_line++;
                std::istringstream iss(line);
                if(parse_line(iss,parsed_line)){
                    switch (parsed_line.character[0]){
                            case 'v':
                                vertices.push_back(parse_vertex(parsed_line));
                                break;
                            case 'f':
                                break;
                            default:
                                throw std::invalid_argument("Invalid line in obj file");
                    }
                }
                else{
                    invalid_reads++;
                }
            }
            myfile.close();
        }
    }
    else{
        auto ss = std::stringstream{fname};
        line_item parsed_line;
        for (std::string line; std::getline(ss, line, '\n');){
            current_line++;
            std::istringstream iss(line);
            if(parse_line(iss, parsed_line)){
                switch (parsed_line.character[0]){
                    case 'v':
                        vertices.push_back(parse_vertex(parsed_line));
                        break;
                    case 'f':
                        break;
                    default:
                        throw std::invalid_argument("Invalid line in obj file");
                }
            }
            else{
                invalid_reads++;
            }
        }
    }
    return nullptr;
}

bool Parser::parse_line(std::istringstream& iss, struct line_item& out_param){
    out_param.points.clear();
    iss >> out_param.character;
    if(iss.fail()){return false;}
    if(out_param.character != "v"  && out_param.character != "f"){return false;}
    double temp_num;
    while(iss >> temp_num || !iss.eof()){
        if(iss.fail()){return false;}
        out_param.points.push_back(temp_num);
    }
    return true;
}

Tuple Parser::parse_vertex(const line_item& input) const{
    if(input.character!="v"){throw std::invalid_argument("Invalid vertex line");}
    if(input.points.size() != 3){throw std::invalid_argument("Invalid number of points");}
    return GenPoint(input.points[0],input.points[1],input.points[2]);
}

unsigned long Parser::get_invalid_reads() const{
    return this->invalid_reads;
}

unsigned long Parser::get_current_line() const{
    return this->current_line;
}


Tuple Parser::get_vertex(int i) const{
    if(i<=0){throw std::invalid_argument("Out of bounds on parser (0 is out of bounds)");}
    return vertices[i];
}

unsigned long Parser::get_total_vertices() const{
    return this->vertices.size();
}

void Parser::reset_vertices(){
    vertices.clear();
    vertices.push_back(GenPoint(0,0,0));
}
