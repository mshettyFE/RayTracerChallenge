#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include "OBJParser.h"
#include "Triangle.h"
#include "Group.h"
#include "Tuple.h"

void Parser::read_line(const std::string& line){
    current_line++;
    std::istringstream iss(line);
    std::string type;
    iss >> type;
    if(iss.fail()){
        throw InvalidLineParser();
    }
    if(type=="v"){
        parse_vertex(iss);
    }
    else if(type=="f"){
        parse_face(iss);
    }
    else if(type=="g"){
        throw std::invalid_argument("not implemented");
    }
    else if(type=="vn"){
        throw std::invalid_argument("not implemented");
    }
    else{
        throw InvalidLineParser();
    }
}

void Parser::read(const std::string& fname, bool file){
    invalid_reads = 0;
    current_line = 0;
    if(file){
// for now, just assume that fname is valid filename
        std::ifstream myfile(fname.c_str());
        if (myfile.is_open()){
            std::string line;
            while(std::getline(myfile, line)){
                try{
                    read_line(line);
                }
                catch (const InvalidLineParser & e) {
                        invalid_reads++;
                }
            }
            myfile.close();
        }
        else{
            throw std::invalid_argument("Bad file name");
        }
    }
    else{
        auto ss = std::stringstream{fname};
        for (std::string line; std::getline(ss, line, '\n');){
                try{
                    read_line(line);
                }
                catch (const InvalidLineParser &e) {
                        invalid_reads++;
                }
        }
    }
}

void Parser::parse_face(std::istringstream& iss){
    std::string error_message = "Error at "+current_line;
    bool accept_normals = true;
    if(normals.size() == 1){
        accept_normals = false;
    }
    std::vector<std::string> verts;
    std::string temp;
    while(iss >> temp || !iss.eof()){
        if(iss.fail()){throw std::invalid_argument(error_message);}
        verts.push_back(temp);
    }
// parse snippets of face line
    std::vector<int> recorded_vertices{0};
    std::vector<int> recorded_normals{0};
    for(std::string item: verts){
        if(accept_normals){
            throw std::invalid_argument("Not Implemented");           
        }
// parse simple faces
        else{
            recorded_vertices.push_back(std::stoi(item));
        }
    }
// Record triangles
    if(recorded_vertices.size() < 3){throw std::invalid_argument(error_message);}
    int first_node = recorded_vertices[1];
    Tuple p1 = get_vertex(first_node);
    for(int i=2; i<recorded_vertices.size()-1; ++i){
        int second_node = recorded_vertices[i];
        int third_node = recorded_vertices[i+1];
        Tuple p2 = get_vertex(second_node); 
        Tuple p3 = get_vertex(third_node);
        std::string triangle_name = gen_triangle_id();
        recorded_groups.insert(std::make_pair(triangle_name,std::make_unique<Triangle>(std::move(Triangle({p1,p2,p3})))));
    }
}

std::string Parser::gen_triangle_id(){
std::string candidate_id;
    while(true){
        candidate_id = "Triangle_"+std::to_string(Triangle_ID);
        Triangle_ID++;
        if (recorded_groups.count(candidate_id)) {
            continue;
        } else {
            break;
        }
    }
    return candidate_id;
}

void Parser::parse_vertex(std::istringstream& iss){
    std::string error_message = "Error at "+current_line;
    double temp_num;
    std::vector<double> points;
    while(iss >> temp_num || !iss.eof()){
        if(iss.fail()){throw std::invalid_argument(error_message);}
        points.push_back(temp_num);
    }
    if(points.size() != 3){throw std::invalid_argument(error_message);}
    vertices.push_back(GenPoint(points[0],points[1],points[2]));
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

std::unique_ptr<Group> Parser::emit(){
    auto g = std::make_unique<Group>(Group());
    for(auto &pair : recorded_groups){
        g->add_child(std::move(pair.second));
    }
    recorded_groups.clear();
    vertices.clear();
    normals.clear();
    vertices.push_back(GenPoint(0,0,0));
    normals.push_back(GenVec(0,0,0));

    return g;
}

    const std::vector<Tuple> Parser::get_vertices() const{
        return vertices;
    }
