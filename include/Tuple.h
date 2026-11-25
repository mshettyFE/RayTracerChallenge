#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <array>
#include <iostream>
#include <initializer_list>
#include <cmath>
#include "Matrix.h"
#include "Constants.h"

class Tuple{
  protected:
    // number of physical dimensions
    static constexpr int dim = 3;
// x, y, z, then w: always either 0 (Vector) or 1 (Point)
    std::array<double, dim+1> data;

    bool is_same(double one, double two) const;
    friend class Matrix;

  public:
    // Constructors
    // Defaults to internally being TupType::VECTOR unless constructed as Point
    // The zero vector.
    Tuple();
    // Direct construction from coordinates, without the initializer_list size check and loop.
    Tuple(double x, double y, double z, TupType a_type=TupType::VECTOR)
      : data{x, y, z, static_cast<double>(a_type)}{}
    // args must be exactly 3 coordinates (x, y, z); throws otherwise.
    Tuple(std::initializer_list<double> args,  const TupType a_type=TupType::VECTOR);
    Tuple(std::vector<double> args,  const TupType a_type=TupType::VECTOR);

    // overloaded binary operators
    bool operator==(const Tuple& other) const;
    bool operator!=(const Tuple& other) const;

    Tuple& operator+=(const Tuple& other);
    Tuple operator+(const Tuple& other) const;

    Tuple& operator-=(const Tuple& other);
    Tuple operator-(const Tuple& other) const;

    double operator[](int index) const;

    // negation
    Tuple operator-() const;

    // templated overloads need to be in header to be in same translation unit
    template <  typename T>
    Tuple operator*=(T scalar){
      static_assert(std::is_arithmetic<T>::value,"Need to multiply tuple by a number");
      for(int i=0; i< this->dim; ++i){
        this->data[i] *= scalar;
      }
      return *this;
    }

    template <  typename T>
    Tuple operator*(T scalar) const{
      static_assert(std::is_arithmetic<T>::value,"Need to multiply tuple by a number");
      Tuple out = *this;
      for(int i=0; i< this->dim; ++i){
        out.data[i] *= scalar;
      }
      return out;
    }

    // Does in place normalization of vector
    void normalize(){
      // normalize physical dimensions (ie. leave w alone)
      const double length = std::sqrt(L2Norm());
      for(int i=0; i< dim; ++i){
        data[i] = (length < glob_resolution) ? 0.0 : data[i]/length;
      }
    }
    // Returns Euclidean length squared
    double L2Norm() const{
      double sum = 0.0;
      for(int i=0; i< dim; ++i){
        sum += data[i]*data[i];
      }
      return sum;
    }

    double dot(const Tuple& other) const;
    Tuple cross(const Tuple& other) const;

    TupType type() const;
    void set_type(TupType t);

    double get_dim() const;

    friend std::ostream& operator << (std::ostream &out, const Tuple& other);
    // Matrix * tuple, including w.
    inline friend Tuple operator* (const Matrix& cur, const Tuple& other){
      Tuple out;
      for(int i=0; i< dim+1; ++i){   // 4 rows: matrices are always 4x4
        double sum = 0.0;
        for(int j=0; j< dim+1; ++j){
          const double new_add = other.data[j]*cur(i,j);
          // 0 * infinity is NaN; treat it as 0 (needed for unbounded shapes' boxes)
          if(!std::isnan(new_add)){
            sum += new_add;
          }
        }
        out.data[i] = sum;
      }
      return out;
    }

    Tuple reflect(const Tuple& normal) const;

    double get_x() const{return this->data[0];};
    double get_y() const{return this->data[1];};
    double get_z() const{return this->data[2];};

};

template <typename T>
Tuple operator*(T scalar, Tuple const & other) {
    static_assert(std::is_arithmetic<T>::value,"Need to multiply tuple by a number");
    return other * scalar;
}

inline Tuple normalize(const Tuple& other){
  Tuple out = other;
  out.normalize();
  return out;
}

Tuple GenVec(double x,double y,double z);
Tuple GenPoint(double x,double y,double z);

#endif
