#include "Matrix.h"
#include <stdexcept>

Matrix::Matrix(int a_dim, double a_resolution){
    if(a_dim<=0){
        throw std::invalid_argument("Matrix dimension must be strictly positive");
    }
    resolution = a_resolution;
    dim = a_dim;
    for(int i=0; i< a_dim; ++i){
        for(int j=0; j<a_dim; ++j){
            data.push_back(0);
        }
    }
}

Matrix::Matrix(int a_dim, std::initializer_list<std::initializer_list<double>> args, double a_resolution){
    if(a_dim<=0){
        throw std::invalid_argument("Matrix dimension must be strictly positive");
    }
    if(args.size() != (a_dim)){
        throw std::invalid_argument("Initialization list must have same number of rows as dim");
    }
    for(auto row : args){
        if(row.size() != a_dim){
            throw std::invalid_argument("Initialization list must have same number of columns as dim");
        }
    }
    resolution = a_resolution;
    dim = a_dim;
    for(auto row: args){
        for(auto entry: row){
            data.push_back(entry);
        }
    }
}

int Matrix::get_dim() const{
    return dim;
}

int Matrix::get_index (unsigned int row, unsigned int col) const{
    if (row>= dim){
        throw std::invalid_argument("Invalid row access");
    }
    if (col>= dim){
        throw std::invalid_argument("Invalid column access");
    }
    return row*dim+col;
}

double Matrix::operator() (unsigned int row, unsigned int col) const{
    return data[get_index(row, col)];
}

bool Matrix::operator==(const Matrix& other) const{
    if (other.dim != this->dim){
        throw std::invalid_argument("matrix dimensions don't match");
    }
    for(int i=0; i<other.dim; ++i){
        for(int j=0; j<other.dim; ++j){
            if(!is_same(this->data[get_index(i,j)], other(i,j))){
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const{
    if (other.dim != this->dim){
        throw std::invalid_argument("matrix dimensions don't match");
    }
    for(int i=0; i<other.dim; ++i){
        for(int j=0; j<other.dim; ++j){
            if(is_same(this->data[get_index(i,j)], other(i,j))){
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::operator*(const Matrix& other) const{
    if (other.dim != this->dim){
        throw std::invalid_argument("matrix dimensions don't match");
    }
    Matrix out(other.dim);
    for(int i=0; i<this->dim; ++i){
        for(int j=0; j<other.dim; ++j){
            double sum = 0.0;
            for(int k=0; k<other.dim; ++k ){
                sum += this->data[get_index(i,k)]*other.data[get_index(k,j)];
            }
            out.data[get_index(i,j)] = sum;
        }
    }
    return out;
}

bool Matrix::is_same(double one, double two) const{
    if(std::abs(one-two) < resolution){
        return true;
    }
    return false;
}

Matrix Matrix::Transpose() const{
    Matrix out(dim);
    for(int i=0; i< dim; ++i){
        for(int j=0; j< dim; ++j){
            out.data[get_index(i,j)] = this->data[get_index(j,i)];
        }
    }
    return out;
}


Matrix MatIdentity(int a_dim){
    if(a_dim<=0){
        throw std::invalid_argument("Matrix dimension must be strictly positive");
    }
    Matrix out(a_dim);
    for(int i=0; i<a_dim; ++i){
        out.data[out.get_index(i,i)] = 1;
    }
    return out;
}

std::ostream& operator << (std::ostream &out, const Matrix& other){
    for(int i=0; i<other.get_dim(); ++i){
        for(int j=0; j<other.get_dim(); ++j){
            out << other(i,j) << " ";
        }
        out << "\n";
    }
    return out;
}
