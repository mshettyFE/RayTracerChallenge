#include "Constants.h"
#include "Tuple.h"
#include "Matrix.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

Tuple::Tuple(){
  // Empty tuple
  dim = 1;
  resolution = glob_resolution;
  data.push_back(0);
  data.push_back(TupType::VECTOR);
}

Tuple::Tuple(std::initializer_list<double> args, const TupType a_type, double a_resolution){
  for (double item: args){
    data.push_back(item);
  }
  dim = data.size();
  resolution = a_resolution;
  data.push_back(a_type);
}

Tuple::Tuple(std::vector<double> args,  const TupType a_type, double a_resolution){
  for (double item: args){
    data.push_back(item);
  }
  dim = data.size();
  resolution = a_resolution;
  data.push_back(a_type);
}


Tuple::Tuple(unsigned int a_dim, const TupType a_type, double a_resolution){
  for(int i=0; i< a_dim; ++i){
    data.push_back(0);
  }
  dim = data.size();
  resolution = a_resolution;
  data.push_back(a_type);
}

double Tuple::get_resolution() const{return resolution;}
double Tuple::get_dim() const{return dim;}


bool Tuple::operator==(const Tuple& other) const {
  if(this->data.size() != other.data.size()){
    return false;
  }
  for(int i=0; i< this->data.size(); ++i){
    if(!is_same(this->data[i], other.data[i])){
      return false;
    }
  }
  return true;
}

bool Tuple::operator!=(const Tuple& other) const {
  if(this->data.size() != other.data.size()){
    return true;
  }
  for(int i=0; i< this->data.size(); ++i){
    if(!is_same(this->data[i], other.data[i])){
      return true;
    }
  }
  return false;
}


Tuple Tuple::operator+=(const Tuple& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  // Remember that this->dim represents the number of physical dimensions. The true length of data is dim+1.
  // Hence, the index of the last element (the w component) is dim
  if(is_same(this->data[this->dim],TupType::POINT) && is_same(other.data[other.dim],TupType::POINT)){
    throw std::invalid_argument("Can't Add points together");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] += other.data[i];
  }
  return *this;
}

Tuple Tuple::operator+(const Tuple& other) const{
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  if(is_same(this->data[this->dim],TupType::POINT) && is_same(other.data[other.dim],TupType::POINT)){
    throw std::invalid_argument("Can't Add points together");
  }
  Tuple out = other;
  for(int i=0; i< other.data.size(); ++i){
    out.data[i] += this->data[i];
  }
  return out;
}

Tuple Tuple::operator-=(const Tuple& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] -= other.data[i];
  }
  return *this;
}

Tuple Tuple::operator-(const Tuple& other) const{
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  Tuple out = *this;
  for(int i=0; i< other.data.size(); ++i){
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

Tuple Tuple::operator-(){
  Tuple out = *this;
  for(int i=0; i< this->dim; ++i){
    out.data[i] = -out.data[i];
  }
  return out;
}


void Tuple::normalize(){
  // normalize physical dimensions (ie. leave last entry of data alone)
  double scaling = L2Norm();
  scaling = pow(scaling, 0.5);
  for(int i=0; i< this->dim; ++i){
    this->data[i] = this->data[i]/scaling;
  }
}

double Tuple::L2Norm(){
  // Return L2Norm
  double scaling = 0.0;
  for(int i=0; i< this->dim; ++i){
    double d = this->data[i];
    scaling += d*d;
  }
  return scaling;
}

double Tuple::dot(const Tuple& other) const{
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  double tally = 0.0;
  for(int i=0; i< this->data.size(); ++i){
    tally += this->data[i]*other.data[i];
  }
  return tally;
}

Tuple Tuple::cross(const Tuple& other) const{
    if (this->data.size() != other.data.size()){
      throw std::invalid_argument("Dimensions don't match");
    }
    if(this->dim != 3){
      throw std::invalid_argument("Dimensions must be 3");
    }
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
  if ( abs(d-TupType::POINT) < resolution){
    return TupType::POINT;
  }
  return TupType::VECTOR;
}

bool Tuple::is_same(double one, double two) const{
  if(std::abs(one-two) < resolution){
    return true;
  }
  return false;
}

Tuple operator* (const Matrix& cur, const Tuple& other){
    double total_dims = other.get_dim()+1;
    if (total_dims != cur.get_dim()){
        throw std::invalid_argument("matrix dimensions don't match");
    }
    Tuple out(other.get_dim());
    for(int i=0; i<cur.get_dim(); ++i){
        double sum = 0.0;
        for(int j=0; j< total_dims; ++j){
            sum += other.data[j]*cur(i,j);
        }
        out.data[i]=  sum;
    }
    return out;
}

std::ostream& operator << (std::ostream &out, const Tuple& other){
  for(int i=0; i<other.data.size(); ++i){
    out << other.data[i] << " ";
  }
  out << '\n';
  return out;
}

Tuple normalize(const Tuple& other){
  Tuple out = other;
  out.normalize();
  return out;
}

Tuple Tuple::reflect(const Tuple& normal) const{
  return *this-normal*2*this->dot(normal); // Subtracts of twice the component in the same direction of normal from the input
}
