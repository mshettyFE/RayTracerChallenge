#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <iostream>
#include <initializer_list>

// Named typed to delinear between Points and Vectors in homogeneous transformations
// w component choosen such that transformations become nice
enum TupType{
  VECTOR = 0, // corresponds to w component of 0
  POINT = 1, // corresponds to w component (ie. last component)  of 1
};

class Tuple{
  private:
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
    Tuple(std::initializer_list<double> args,  TupType a_type=TupType::VECTOR, const double resolution=1E-9);
    Tuple(unsigned int dim, TupType a_type=TupType::VECTOR,  const double resolution=1E-9);


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
    Tuple operator+(const Tuple& other) const;

    Tuple operator-=(const Tuple& other);
    Tuple operator-(const Tuple& other) const;

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

    // Friend printing methods
    friend std::ostream& operator << (std::ostream &out, const Tuple& other);

};

template <typename T>
Tuple operator*(T scalar, Tuple const & matrix) {
    return matrix * scalar;
}
#endif
