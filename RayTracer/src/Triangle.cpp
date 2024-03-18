#include "Triangle.h"
#include "Ray.h"
#include "Impact.h"
#include <initializer_list>

Triangle::Triangle(const std::initializer_list<double> p1, const std::initializer_list<double> p2,const std::initializer_list<double> p3, const Matrix& Transformation, const Material& material, Shape* parent) : Shape(Transformation,material, parent,"Triangle"){
    if(p1.size() != 3){throw std::invalid_argument("In Triangle, p1 have 3 coordinates");}   
    if(p2.size() != 3){throw std::invalid_argument("In Triangle, p2 have 3 coordinates");}   
    if(p3.size() != 3){throw std::invalid_argument("In Triangle, p3 have 3 coordinates");}
    this->p1 = Tuple(p1,TupType::POINT);
    this->p2 = Tuple(p2,TupType::POINT);
    this->p3 = Tuple(p3,TupType::POINT);
    this->edge1 = this->p2-this->p1;
    this->edge2 = this->p3-this->p1;
    this->normal = this->edge2.cross(this->edge1);
    this->normal.normalize();
}

Triangle::Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Matrix& Transformation, const Material& material, Shape* parent) : Shape(Transformation,material, parent,"Triangle"){
    if(p1.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p1 must have 3 coordinates");}
    if(p2.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p2 must have 3 coordinates");}
    if(p2.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p3 must have 3 coordinates");}
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->edge1 = this->p2-this->p1;
    this->edge2 = this->p3-this->p1;
    this->normal = this->edge2.cross(this->edge1);
    this->normal.normalize();
}


Tuple Triangle::local_normal_at(const Tuple& pt, const Impact& impt) const {
    return this->normal; 
}

std::unique_ptr<AABB> Triangle::bound() const{return nullptr;}


std::vector<Impact> Triangle::local_intersect(const Ray &other) const {
//www.tandfonline.com/doi/abs/10.1080/10867651.1997.10487468
    auto crossed_e2 = other.get_direction().cross(edge2);
    double det = edge1.dot(crossed_e2);
    if(std::abs(det) < glob_resolution){
        return {};
    }
    double f = 1.0/det;
    Tuple p1_to_origin = other.get_origin()-p1;
    double u = f*p1_to_origin.dot(crossed_e2);
    if((u <0) || (u>1)){return{};}
    auto crossed_e1 = p1_to_origin.cross(edge1);
    double v = f*other.get_direction().dot(crossed_e1);
    if((v <0) || (u+v>1)){return{};}
    double t = f*edge2.dot(crossed_e1);
    return {Impact(t,this)};
}

void Triangle::verbose_print() const {
    std::cout << name << std::endl;
    std::cout << "Points" << std::endl;
    std::cout << p1 << p2 << p3 << std::endl;
    std::cout << "Edges" << std::endl;
    std::cout << edge1 << edge2 << std::endl;
    std::cout << "Normal" << std::endl;
    std::cout << normal << std::endl;
}

std::vector<Tuple> Triangle::get_points() const{
    return{p1,p2,p3};
}

Tuple Triangle::get_edge1() const{
    return edge1;
}
Tuple Triangle::get_edge2() const{
    return edge2;
}
