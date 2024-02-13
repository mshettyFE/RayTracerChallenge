#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>
#include "Constants.h"
#include <vector>

class Matrix{
    protected:
        int dim;
        double resolution;
        bool is_same(double one, double two) const;
        friend class Tuple;
    public:
        std::vector<double> data;
        Matrix(int dim, double resolution=glob_resolution);
        // assumes row, column notation
        Matrix(int a_dim, std::initializer_list<std::initializer_list<double>> args, double a_resolution=glob_resolution);

        int get_index(unsigned int x, unsigned int y) const;

        int get_dim() const;

        double operator()(unsigned int row, unsigned int col) const;
        bool operator==(const Matrix& other) const;
        bool operator!=(const Matrix& other) const;
        Matrix operator*(const Matrix& other) const;

        Matrix Transpose() const;

        template <  typename T>
        Matrix operator*(T scalar) const{
            Matrix out = *this;
            for(int i=0; i< this->dim; ++i){
                for(int j=0; j< this->dim; ++j){
                    out.data[get_index(i,j)] *= scalar;
                }
            }
            return out;
        }

    friend std::ostream& operator << (std::ostream &out, const Matrix& other);
};

template <typename T>
Matrix operator*(T scalar, Matrix const & other) {
    return other * scalar;
}

Matrix MatIdentity(int a_dim);
#endif