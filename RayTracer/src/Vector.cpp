#include "Vector.h"
#include <stdexcept>

Vector::Vector(std::initializer_list<double> args){
  for (double item: args){
    data.push_back(item);
  }
}

Vector::Vector(unsigned int dim){
  for(int i=0; i< dim; ++i){
    data.push_back(0);
  }
}

Vector::Vector (const Vector& other){
  for(int i=0; i< other.data.size(); ++i){
    data.push_back(other.data[i]);
  }
}

Vector& Vector::operator=(const Vector& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  this->data.clear();
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] = other.data[i];
  }
  return *this;
}

Vector::Vector (Vector&& other){
  other.data.clear();
}

Vector& Vector::operator=(Vector&& other){
  std::swap(data,other.data);
  return *this;
}

Vector Vector::operator+=(const Vector& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] += other.data[i];
  }
  return *this;
}

Vector Vector::operator+(const Vector& other){
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  Vector out = other;
  for(int i=0; i< other.data.size(); ++i){
    out.data[i] += this->data[i];
  }
  return out;
}

Vector Vector::operator-=(const Vector& other){
  if(this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  for(int i=0; i< this->data.size(); ++i){
    this->data[i] -= other.data[i];
  }
  return *this;
}

Vector Vector::operator-(const Vector& other){
  if (this->data.size() != other.data.size()){
    throw std::invalid_argument("Dimensions don't match");
  }
  Vector out = *this;
  for(int i=0; i< other.data.size(); ++i){
    out.data[i] -= other.data[i];
  }
  return out;
}

std::ostream & operator << (std::ostream &out, const Vector& other){
  for(int i=0; i<other.data.size(); ++i){
    out << other.data[i] << " ";
  }
  out << '\n';
  return out;
}

