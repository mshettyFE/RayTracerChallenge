#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>
#include <type_traits>
#include "Constants.h"
#include <vector>

class Matrix{
    protected:
        // Tacitly assumes square matrices, since that is the only type of matrix I care about for Ray Tracing (I think...)
        // hence, dim denotes a dimxdim matrix
        int dim;
        double resolution;
        // utility function to check if two numbers are the same
        bool is_same(double one, double two) const;
        // Allow Tuple class direct access to protected members of Matrix
        friend class Tuple;
    public:
        std::vector<double> data;
        Matrix();
        Matrix(int dim, double resolution=glob_resolution);
        // assumes row, column notation
        Matrix(std::initializer_list<std::initializer_list<double>> args, double a_resolution=glob_resolution);

        // Just like Canvas implementation
        int get_index(unsigned int x, unsigned int y) const;

        int get_dim() const;

        // utilized in inverse matrix calculation. Done in place
        void swap_row(int old_row, int new_row);
        void scale(int row, double scaling);

        // Operator overloads for convinience
        double operator()(unsigned int row, unsigned int col) const;
        bool operator==(const Matrix& other) const;
        bool operator!=(const Matrix& other) const;
        Matrix operator*(const Matrix& other) const;

        Matrix Transpose() const;
        Matrix Inverse(bool debug=false) const;

        // This needs to be here in header due to translational units (I think...)
        template <  typename T>
        Matrix operator*(T scalar) const{
            static_assert(std::is_arithmetic<T>::value,"Need to multiply matrix by a number");
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

// This needs to be declared outside class to allow left multiplication (overloading operator* only allows right multiplication)
template <typename T>
Matrix operator*(T scalar, Matrix const & other) {
    static_assert(std::is_arithmetic<T>::value,"Need to multiply matrix by a number");
    return other * scalar;
}

// Helper functions to generate Matrices for Specific purposes
Matrix MatIdentity(int a_dim);
// All of these assume a 4x4 matrix
Matrix MatTranslation(double x, double y, double z);
Matrix MatScaling(double x, double y, double z);
Matrix MatRotateX(double angle);
Matrix MatRotateY(double angle);
Matrix MatRotateZ(double angle);
Matrix MatShear(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y);

Matrix Chain(std::vector<Matrix> transformations);

#endif