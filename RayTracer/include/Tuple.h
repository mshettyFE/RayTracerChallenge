#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <iostream>
#include <initializer_list>
#include "Constants.h"

class Tuple{
  protected:
// How close two doubles need to be to be overlapping
    double resolution;
// The number of physical dimensions. The actual number of coordinates stored is dim+1 to account for point/vector info
    int dim;
// place where data is stored
    std::vector<double> data;

    bool is_same(double one, double two) const;

  public:
    // Constructors
    // Defaults to internally being Vectors unless constructed as Point
    Tuple();
    Tuple(std::initializer_list<double> args,  const TupType a_type=TupType::VECTOR, double resolution=glob_resolution);
    Tuple(unsigned int dim, const TupType a_type=TupType::VECTOR,  double resolution=glob_resolution);

    // overloaded binary operators
    bool operator==(const Tuple& other) const;
    bool operator!=(const Tuple& other) const;

    Tuple operator+=(const Tuple& other);
    Tuple operator+(const Tuple& other) const;

    Tuple operator-=(const Tuple& other);
    Tuple operator-(const Tuple& other) const;

    double operator[](int index);

    // negation
    Tuple operator-();

    // templated overloads need to be in header to be in same translation unit
    template <  typename T>
    Tuple operator*=(T scalar){
      for(int i=0; i< this->dim; ++i){
        this->data[i] *= scalar;
      }
      return *this;
    }

    template <  typename T>
    Tuple operator*(T scalar) const{
      Tuple out = *this;
      for(int i=0; i< this->dim; ++i){
        out.data[i] *= scalar;
      }
      return out;
    }

    // Does in place normalization of vector
    void normalize();
    // Returns Euclidean length squared
    double L2Norm();

    double dot(const Tuple& other) const;
    Tuple cross(const Tuple& other) const;

    TupType type() const;

    const double get_resolution(){return resolution;};
    const double get_dim(){return dim;};

    // Friend printing methods
    friend std::ostream& operator << (std::ostream &out, const Tuple& other);

};

template <typename T>
Tuple operator*(T scalar, Tuple const & other) {
    return other * scalar;
}
#endif
