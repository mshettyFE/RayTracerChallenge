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

void Matrix::swap_row(int old_row, int new_row){
// in place swapping of rows
    if( (old_row < 0) || (new_row< 0) || (old_row >= dim) || (new_row >= dim) ){
        throw std::invalid_argument("Out of bounds array");
    }
    for(int j=0; j< dim; ++j){
        double temp = this->data[get_index(old_row,j)];
        this->data[get_index(old_row,j)] = this->data[get_index(new_row,j)];
        this->data[get_index(new_row,j)] = temp;
    }
}

void Matrix::scale(int row, double scaling){
    if( (row < 0) ||  (row >= dim)){
        throw std::invalid_argument("Out of bounds array");
    }
    for(int j=0; j< dim; ++j){
        this->data[get_index(row,j)] *= scaling;
    }
}

Matrix Matrix::Inverse(bool debug){
    Matrix in = *this;
    Matrix out = MatIdentity(dim);
    for(int col=0; col<dim; ++col){
        if(debug){
            std::cout << "Start of loop " << col << std::endl;
            std::cout << in << std::endl;
            std::cout << out << std::endl;
        }
        double pivot = in.data[get_index(col,col)];
        if(is_same(pivot,0)){
            bool new_pivot_found = false;
            // look at rows below current invalid pivot
            for(int row=col+1; row<dim; ++row){
                double new_pivot = in.data[get_index(row,col)];
                if(!is_same(new_pivot,0)){
                    // Move new pivot row to cur row
                    in.swap_row(col,row);
                    out.swap_row(col,row);
                    pivot = new_pivot;
                    new_pivot_found = true;
                    break;
                }
            }
            if(!new_pivot_found){
                throw std::domain_error("Inverse does not exist");
            }
            if(debug){
                std::cout << "Swapped "  << std::endl;
                std::cout << in << std::endl;
                std::cout << out << std::endl;
            }
        }
        // scale the row so that the pivot element is 1
        in.scale(col,1.0/pivot);
        out.scale(col, 1.0/pivot);
        if(debug){
            std::cout << "Scaled "  << std::endl;
            std::cout << in << std::endl;
            std::cout << out << std::endl;
        }
        //pivot found, need to reduce all rows above and below current row
        for(int row=0; row<dim; ++row){
            if(row==col){
                continue;
            }
            // get the element in the same column as pivot element and negate.
            double scaling = -1.0*in.data[get_index(row,col)];
            for(int sweep=0; sweep<dim; ++sweep){
                // Do the row elimination
                in.data[get_index(row,sweep)] += in.data[get_index(col,sweep)]*scaling;
                out.data[get_index(row,sweep)] += out.data[get_index(col,sweep)]*scaling;
            }
        }
        if(debug){
            std::cout << "Added "  << std::endl;
            std::cout << in << std::endl;
            std::cout << out << std::endl;
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
