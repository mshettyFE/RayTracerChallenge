#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>
#include "Constants.h"
#include <vector>

class Matrix{
    private:
        int dim;
        double resolution;
    public:
        std::vector<double> data;
        Matrix(int dim, double resolution=glob_resolution);
        // assumes row, column notation
        Matrix(int a_dim, std::initializer_list<std::initializer_list<double>> args, double a_resolution=glob_resolution);

        int get_index(unsigned int x, unsigned int y) const;

        int get_dim() const;

        double operator()(unsigned int row, unsigned int col) const;


    friend std::ostream& operator << (std::ostream &out, const Matrix& other);
};

#endif