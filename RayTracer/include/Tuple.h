#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <iostream>
#include <initializer_list>

// Named typed to delinear between Points and Vectors in homogeneous transformations
// w component choosen such that transformations become nice
enum VType{
  VECTOR = 0, // corresponds to w component of 0
  POINT = 1, // corresponds to w component (ie. last component)  of 1
};

class Tuple{
  private:
// How close two doubles need to be to be overlapping
    double resolution;
// The number of physical dimensions. The actual number of coordinates stored is dim+1 to account for point/vector info
    int dim;

  public:
    std::vector<double> data;
    
    // Constructors
    // Defaults to internally being Vectors unless constructed as Point
    Tuple(std::initializer_list<double> args, const double resolution=1E-9, VType a_type=VType::VECTOR);
    Tuple(unsigned int dim, const double resolution=1E-9, VType a_type=VType::VECTOR);


    // copy constructors
    Tuple (const Tuple& other);
    Tuple& operator=(const Tuple& other);
    // move constructors
    Tuple (Tuple&& other);
    Tuple& operator=(Tuple&& other);

    // overloaded binary operators
    bool operator==(const Tuple& other) const;
    bool operator!=(const Tuple& other) const;

    Tuple operator+=(const Tuple& other);
    Tuple operator+(const Tuple& other);

    Tuple operator-=(const Tuple& other);
    Tuple operator-(const Tuple& other);

    // templated overloads need to be in header to be in same translation unit
    template <  typename T>
    Tuple operator*=(T scalar){
      for(int i=0; i< this->data.size(); ++i){
        this->data[i] *= scalar;
      }
      return *this;
    }

    template <  typename T>
    Tuple operator*(T scalar){
      Tuple out = *this;
      for(int i=0; i< this->data.size(); ++i){
        out.data[i] *= scalar;
      }
      return out;
    }

    // Does in place normalization of vector
    void normalize();

    // Friend printing methods
    friend std::ostream& operator << (std::ostream &out, const Tuple& other);

};

#endif
