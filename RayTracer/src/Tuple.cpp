#include "Tuple.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

Tuple::Tuple(std::initializer_list<double> args, const double a_resolution, VType a_type){
  for (double item: args){
    data.push_back(item);
  }
  dim = data.size();
  resolution = a_resolution;
  data.push_back(a_type);
}

Tuple::Tuple(unsigned int a_dim, const double a_resolution, VType a_type){
  for(int i=0; i< a_dim; ++i){
    data.push_back(0);
  }
  dim = data.size();
  resolution = a_resolution;
  data.push_back(a_type);
}

Tuple::Tuple (const Tuple& other){
  for(int i=0; i< other.data.size(); ++i){
    data.push_back(other.data[i]);
  }
  resolution = other.resolution;
  dim = other.dim;
}

Tuple& Tuple::operator=(const Tuple& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  this->data.clear();
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] = other.data[i];
  }
  return *this;
}

Tuple::Tuple (Tuple&& other){
  other.data.clear();
}

Tuple& Tuple::operator=(Tuple&& other){
  std::swap(data,other.data);
  return *this;
}

bool Tuple::operator==(const Tuple& other) const {
  if(this->data.size() != other.data.size()){
    return false;
  }
  for(int i=0; i< this->data.size(); ++i){
    if( abs(this->data[i]-other.data[i]) > this->resolution){
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
    if( abs(this->data[i]-other.data[i]) > this->resolution){
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
  if( (abs(this->data[this->dim]-VType::POINT) <resolution) && abs(other.data[other.dim]-VType::POINT) <resolution){
    throw std::invalid_argument("Can't Add points together");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] += other.data[i];
  }
  return *this;
}

Tuple Tuple::operator+(const Tuple& other){
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  if( (abs(this->data[this->dim]-VType::POINT) <resolution) && abs(other.data[other.dim]-VType::POINT) <resolution){
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
  if( (abs(this->data[this->dim]-VType::POINT) <resolution) && abs(other.data[other.dim]-VType::POINT) <resolution){
    throw std::invalid_argument("Can't Subtract points together");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] -= other.data[i];
  }
  return *this;
}

Tuple Tuple::operator-(const Tuple& other){
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  if( (abs(this->data[this->dim]-VType::POINT) <resolution) && abs(other.data[other.dim]-VType::POINT) <resolution){
    throw std::invalid_argument("Can't Subtract points together");
  }
  Tuple out = *this;
  for(int i=0; i< other.data.size(); ++i){
    out.data[i] -= other.data[i];
  }
  return out;
}

void Tuple::normalize(){
  double scaling = 0.0;
  for(int i=0; i< this->dim; ++i){
    double d = this->data[i];
    scaling += d*d;
  }
  scaling = pow(scaling, 0.5);
  for(int i=0; i< this->dim; ++i){
    this->data[i] = this->data[i]/scaling;
  }
}

std::ostream& operator << (std::ostream &out, const Tuple& other){
  for(int i=0; i<other.data.size(); ++i){
    out << other.data[i] << " ";
  }
  out << '\n';
  return out;
}