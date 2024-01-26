#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <iostream>
#include <initializer_list>

class Vector{
  public:
    std::vector<double> data;    
    
    // Constructors
    Vector(std::initializer_list<double> args);
    Vector(unsigned int dim);

    // copy constructors
    Vector (const Vector& other);
    Vector& operator=(const Vector& other);
    // move constructors
    Vector (Vector&& other);
    Vector& operator=(Vector&& other);

    Vector operator+=(const Vector& other);
    Vector operator+(const Vector& other);

    Vector operator-=(const Vector& other);
    Vector operator-(const Vector& other);

    template <  typename T>
    Vector operator*=(T scalar){
      for(int i=0; i< this->data.size(); ++i){
        this->data[i] *= scalar;
      }
      return *this;
    }

    template <  typename T>
    Vector operator*(T scalar){
      Vector out = *this;
      for(int i=0; i< this->data.size(); ++i){
        out.data[i] *= scalar;
      }
      return out;
    }


    // Friend printing class
    friend std::ostream& operator << (std::ostream &out, const Vector& other);

};

#endif
