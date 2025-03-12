#include "SmoothTriangle.h"
#include "Shape.h"

SmoothTriangle::SmoothTriangle(const std::initializer_list<double> p1, const std::initializer_list<double> p2,const std::initializer_list<double> p3,const std::initializer_list<double> n1, const std::initializer_list<double> n2,const std::initializer_list<double> n3, const Matrix& Transformation, const Material& material, Shape* parent): Shape(Transformation,material,parent, "SmoothTriangle"){
    if(p1.size() != 3){throw std::invalid_argument("In Triangle, p1 have 3 coordinates");}   
    if(p2.size() != 3){throw std::invalid_argument("In Triangle, p2 have 3 coordinates");}   
    if(p3.size() != 3){throw std::invalid_argument("In Triangle, p3 have 3 coordinates");}
    if(n1.size() != 3){throw std::invalid_argument("In Triangle, n1 have 3 coordinates");}   
    if(n2.size() != 3){throw std::invalid_argument("In Triangle, n2 have 3 coordinates");}   
    if(n3.size() != 3){throw std::invalid_argument("In Triangle, n3 have 3 coordinates");}
    this->p1 = Tuple(p1,TupType::POINT);
    this->p2 = Tuple(p2,TupType::POINT);
    this->p3 = Tuple(p3,TupType::POINT);
    this->edge1 = this->p2-this->p1;
    this->edge2 = this->p3-this->p1;
    this->n1 = Tuple(n1,TupType::VECTOR);
    this->n2 = Tuple(n2,TupType::VECTOR);
    this->n3 = Tuple(n3,TupType::VECTOR);
    this->n1.normalize();
    this->n2.normalize();
    this->n3.normalize();
}

SmoothTriangle::SmoothTriangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Tuple& n1, const Tuple& n2, const Tuple& n3, const Matrix& Transformation, const Material& material, Shape* parent){
    if(p1.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p1 must be a point");}
    if(p2.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p2 must be a point");}
    if(p2.type() != TupType::POINT){throw std::invalid_argument("In Smooth Triangle, p3 must be a point");}
    if(n1.type() != TupType::VECTOR){throw std::invalid_argument("In Smooth Triangle, n1 must be a vector");}
    if(n2.type() != TupType::VECTOR){throw std::invalid_argument("In Smooth Triangle, n2 must be a vector");}
    if(n3.type() != TupType::VECTOR){throw std::invalid_argument("In Smooth Triangle, n3 must be a vector");}
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->edge1 = this->p2-this->p1;
    this->edge2 = this->p3-this->p1;
    this->n1 = n1;
    this->n2 = n2;
    this->n3 = n3;
    this->n1.normalize();
    this->n2.normalize();
    this->n3.normalize();
}

std::vector<Tuple> SmoothTriangle::get_points() const{
    return {this->p1, this->p2,this->p3};
}

std::vector<Tuple> SmoothTriangle::get_normals() const{
    return {this->p1, this->p2,this->p3};
}

void SmoothTriangle::verbose_print() const {
    std::cout << name << std::endl;
    std::cout << "P1 " <<  this->p1 << std::endl;
    std::cout << "P2 " <<  this->p2 << std::endl;
    std::cout << "P3 " <<  this->p3 << std::endl;
    std::cout << "N1 " <<  this->n1 << std::endl;
    std::cout << "N2 " <<  this->n2 << std::endl;
    std::cout << "N3 " <<  this->n3 << std::endl;
}

std::vector<Impact> SmoothTriangle::local_intersect(const Ray &other) const {
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
    return {Impact(t,this,u,v)};
}

Tuple SmoothTriangle::local_normal_at(const Tuple& pt, const Impact& impt) const {
return this->n2 * impt.get_u() +this->n3 * impt.get_v() + this->n1 * (1 - impt.get_u() - impt.get_v());
}

std::unique_ptr<AABB> SmoothTriangle::bound() const{
    auto output = std::make_unique<AABB>();
    output->add_point(p1);
    output->add_point(p2);
    output->add_point(p3);
    output->set_shape(this);
    return output;
}
