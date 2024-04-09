#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include "OBJParser.h"
#include "Triangle.h"
#include "SmoothTriangle.h"
#include "Group.h"
#include "Tuple.h"

Parser::Parser(bool verbose){
    this->verbose= verbose;
}

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
        parse_group(iss);
    }
    else if(type=="vn"){
        parse_normal(iss);
    }
    else{
        throw InvalidLineParser();
    }
}

void Parser::read(const std::string& fname, bool file){
    invalid_reads = 0;
    current_line = 0;
    current_group="";
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

void Parser::parse_normal(std::istringstream& iss){
    std::string error_message = "Failed to parse. Error at line "+std::to_string(current_line);
    double temp_num;
    std::vector<double> points;
    while(iss >> temp_num || !iss.eof()){
        if(iss.fail()){throw std::invalid_argument(error_message);}
        points.push_back(temp_num);
    }
    if(points.size() != 3){throw std::invalid_argument(error_message);}
    if(verbose){        std::cout <<  "n" <<  std::endl;}
    normals.push_back(GenVec(points[0],points[1],points[2]));
}


void Parser::parse_face(std::istringstream& iss){
    std::string error_message = "Failed to parse. Error at line "+std::to_string(current_line);
    bool accept_normals = true;
    if(normals.size() == 1){
        accept_normals = false;
        error_message += ". Normal deactivated";
    }
    else{
        error_message += ". Normal activated";
    }
    std::vector<std::string> verts;
    std::string temp;
    while(iss >> temp || !iss.eof()){
        if(iss.fail()){throw std::invalid_argument(error_message);}
        verts.push_back(temp);
    }
// parse snippets of face line
    std::vector<int> recorded_vertices;
    std::vector<int> recorded_normals;
    for(std::string item: verts){
        if(accept_normals){
// need to parse files of the form x/*/n, where x is vertex info, n is normal info, and you ignore *
            std::string token;
            int vertex_id=-1;
            int normal_id=-1;
            std::istringstream normal_ss(item);
// vertex
            if(!std::getline(normal_ss, token,'/')){throw std::invalid_argument(error_message);}
            try{vertex_id = std::stoi(token);} catch (std::invalid_argument const& ex){throw std::invalid_argument(error_message);}
// ignore texture index
            if(token.size() == 0){throw std::invalid_argument(error_message);}
            if(!std::getline(normal_ss, token,'/')){throw std::invalid_argument(error_message);}
// get normal index
            normal_ss >> token;
            if(normal_ss.fail() || !normal_ss.eof()){throw std::invalid_argument(error_message);}
            try{normal_id = std::stoi(token);} catch (std::invalid_argument const& ex){throw std::invalid_argument(error_message);}
            recorded_vertices.push_back(vertex_id);
            recorded_normals.push_back(normal_id);
        }
// parse simple faces
        else{
            int vertex_id = -1;
            try{vertex_id = std::stoi(item);} catch (std::invalid_argument const& ex){throw std::invalid_argument(error_message);}
            recorded_vertices.push_back(vertex_id);
        }
    }
// Record triangles
    std::vector<std::unique_ptr<Shape>> recorded_triangles;
    if(recorded_vertices.size() < 3){throw std::invalid_argument(error_message);}
    if(accept_normals){
        if(recorded_vertices.size() != recorded_normals.size()){throw std::invalid_argument(error_message);}
    }
    int first_vertex_node = recorded_vertices[0];
    int first_normal_node=0;
    Tuple p1 = get_vertex(first_vertex_node);
    Tuple n1;
    if(accept_normals){
        first_normal_node = recorded_normals[0];
        n1 = get_normal(first_normal_node);
    }
    for(unsigned int i=1; i<recorded_vertices.size()-1; ++i){
        int second_vertex_node = recorded_vertices[i];
        int third_vertex_node = recorded_vertices[i+1];
        Tuple p2 = get_vertex(second_vertex_node); 
        Tuple p3 = get_vertex(third_vertex_node);
        if(accept_normals){
            int second_normal_node = recorded_normals[i];
            int third_normal_mode = recorded_normals[i+1];
            recorded_triangles.push_back(std::move(std::make_unique<SmoothTriangle>(std::move(SmoothTriangle(vertices[first_vertex_node],vertices[second_vertex_node],vertices[third_vertex_node], normals[first_normal_node], normals[second_normal_node],normals[third_normal_mode])))));
        }
        else{
            recorded_triangles.push_back(std::move(std::make_unique<Triangle>(std::move(Triangle(vertices[first_vertex_node],vertices[second_vertex_node],vertices[third_vertex_node])))));
        }
    }
    std::string group_name;
    if(current_group==""){
        group_name = gen_group_id();
    }
    else{
        group_name = current_group;
    }
    recorded_groups.insert(std::make_pair(group_name,std::make_unique<Group>(Group())));
    for(unsigned int i=0; i< recorded_triangles.size(); ++i){
        recorded_groups[group_name]->add_child(std::move(recorded_triangles[i]));
    }
    if(verbose){        std::cout <<  "f" <<  std::endl;}
}

std::string Parser::gen_group_id(){
std::string candidate_id;
    while(true){
        candidate_id = "Triangle_"+std::to_string(Group_ID);
        Group_ID++;
        if (recorded_groups.count(candidate_id)) {
            continue;
        } else {
            break;
        }
    }
    return candidate_id;
}

void Parser::parse_group(std::istringstream& iss){
    std::string error_message = "Failed to parse. Error at line "+std::to_string(current_line);
    std::string temp;
    iss >> temp;
    if(iss.fail()){throw std::invalid_argument(error_message);}
    if(!iss.eof()){throw std::invalid_argument(error_message);}
    std::string current_group=temp;
    if(verbose){        std::cout <<  "g" << " "<< temp << std::endl;}
}

void Parser::parse_vertex(std::istringstream& iss){
    std::string error_message = "Failed to parse. Error at line "+std::to_string(current_line);
    double temp_num;
    std::vector<double> points;
    while(iss >> temp_num || !iss.eof()){
        if(iss.fail()){throw std::invalid_argument(error_message);}
        points.push_back(temp_num);
    }
    if(points.size() != 3){throw std::invalid_argument(error_message);}
    if(verbose){        std::cout <<  "v" << " "<< GenPoint(points[0],points[1],points[2]) << std::endl;}
    vertices.push_back(GenPoint(points[0],points[1],points[2]));
}

unsigned long Parser::get_invalid_reads() const{
    return this->invalid_reads;
}

unsigned long Parser::get_current_line() const{
    return this->current_line;
}

Tuple Parser::get_vertex(int i) const{
    std::string error_message = std::to_string(i)+" Out of bounds on parser for vertex (0 is out of bounds)";
    if(i<=0){throw std::invalid_argument(error_message);}
    return vertices[i];
}

Tuple Parser::get_normal(int i) const{
    std::string error_message = std::to_string(i)+"Out of bounds on parser for normal (0 is out of bounds)";
    if(i<=0){throw std::invalid_argument(error_message);}
    return normals[i];
}


unsigned long Parser::get_total_vertices() const{
    return this->vertices.size();
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

const std::vector<Tuple> Parser::get_normals() const{
    return normals;
}
