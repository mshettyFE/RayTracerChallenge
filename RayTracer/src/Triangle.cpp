#include "Triangle.h"
#include "Ray.h"
#include "Impact.h"
#include <initializer_list>

Triangle::Triangle(const std::initializer_list<double> p1, const std::initializer_list<double> p2,const std::initializer_list<double> p3, const Matrix& Transformation, const Material& material, std::shared_ptr<Shape> parent) : Shape(Transformation,material, parent,"Triangle"){
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

Tuple Triangle::local_normal_at(const Tuple& pt) const {
    return this->normal; 
}

std::vector<Impact> Triangle::local_intersect(const Ray &other) const {
    auto crossed = other.get_direction().cross(edge2);
    double det = edge1.dot(crossed);
    if(std::abs(det) < glob_resolution){
        return {};
    }
    return {Impact(1,std::make_shared<Triangle>(*this))};
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
