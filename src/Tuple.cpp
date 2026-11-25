#include "Constants.h"
#include "Tuple.h"
#include "Matrix.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

Tuple::Tuple() : data{0, 0, 0, TupType::VECTOR}{}

Tuple::Tuple(std::initializer_list<double> args, const TupType a_type){
  if(args.size() != dim){
    throw std::invalid_argument("A Tuple needs exactly 3 coordinates");
  }
  int i = 0;
  for (double item: args){
    data[i++] = item;
  }
  data[dim] = a_type;
}

Tuple::Tuple(std::vector<double> args,  const TupType a_type){
  if(args.size() != dim){
    throw std::invalid_argument("A Tuple needs exactly 3 coordinates");
  }
  for(int i = 0; i < dim; ++i){
    data[i] = args[i];
  }
  data[dim] = a_type;
}

double Tuple::get_dim() const{return dim;}


bool Tuple::operator==(const Tuple& other) const {
  for(unsigned int i=0; i< this->data.size(); ++i){
    if(!is_same(this->data[i], other.data[i])){
      return false;
    }
  }
  return true;
}

bool Tuple::operator!=(const Tuple& other) const {
  for(unsigned int i=0; i< this->data.size(); ++i){
    if(!is_same(this->data[i], other.data[i])){
      return true;
    }
  }
  return false;
}


Tuple& Tuple::operator+=(const Tuple& other){
  // Remember that this->dim represents the number of physical dimensions. The true length of data is dim+1.
  // Hence, the index of the last element (the w component) is dim
  if(is_same(this->type(),TupType::POINT) && is_same(other.type(),TupType::POINT)){
    throw std::invalid_argument("Can't Add points together");
  }
  for(unsigned int i=0; i< this->data.size(); ++i){
    this->data[i] += other.data[i];
  }
  return *this;
}

Tuple Tuple::operator+(const Tuple& other) const{
  if(is_same(this->data[this->dim],TupType::POINT) && is_same(other.data[other.dim],TupType::POINT)){
    throw std::invalid_argument("Can't Add points together");
  }
  Tuple out = other;
  for(unsigned int i=0; i< other.data.size(); ++i){
    out.data[i] += this->data[i];
  }
  return out;
}

Tuple& Tuple::operator-=(const Tuple& other){
  for(unsigned int i=0; i< this->data.size(); ++i){
    this->data[i] -= other.data[i];
  }
  return *this;
}

Tuple Tuple::operator-(const Tuple& other) const{
  Tuple out = *this;
  for(unsigned int i=0; i< other.data.size(); ++i){
    out.data[i] -= other.data[i];
  }
  return out;
}

double Tuple::operator[](int index) const{
    if(index < 0){
        throw std::invalid_argument("Argument below 0");
    }
    if(index >= 3){
        throw std::invalid_argument("Argument above 2");
    }
    return data[index];
}

Tuple Tuple::operator-() const{
  Tuple out = *this;
  for(int i=0; i< this->dim; ++i){
    out.data[i] = -out.data[i];
  }
  return out;
}


double Tuple::dot(const Tuple& other) const{
  double tally = 0.0;
  for(unsigned int i=0; i< this->data.size(); ++i){
    tally += this->data[i]*other.data[i];
  }
  return tally;
}

Tuple Tuple::cross(const Tuple& other) const{
    double cross_x = this->data[1]*other.data[2]-this->data[2]*other.data[1];
    double cross_y = this->data[2]*other.data[0]-this->data[0]*other.data[2];
    double cross_z = this->data[0]*other.data[1]-this->data[1]*other.data[0];
    Tuple out({cross_x, cross_y, cross_z},TupType::VECTOR);
    return out;
}

void Tuple::set_type(TupType t){
  this->data[this->dim] = t;
}


TupType Tuple::type() const{
  double d = this->data[this->dim];
  if ( std::abs(d-TupType::POINT) < glob_resolution){
    return TupType::POINT;
  }
  return TupType::VECTOR;
}

bool Tuple::is_same(double one, double two) const{
  if(std::abs(one-two) < glob_resolution){
    return true;
  }
  return false;
}

std::ostream& operator << (std::ostream &out, const Tuple& other){
  for(unsigned int i=0; i<other.data.size(); ++i){
    out << other.data[i] << " ";
  }
  out << '\n';
  return out;
}

Tuple Tuple::reflect(const Tuple& normal) const{
  return *this-normal*2*this->dot(normal); // Subtracts of twice the component in the same direction of normal from the input
}

Tuple GenVec(double x,double y,double z){return Tuple(x, y, z, TupType::VECTOR);}
Tuple GenPoint(double x,double y,double z){return Tuple(x, y, z, TupType::POINT);}
