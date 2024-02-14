#include "Matrix.h"
#include <stdexcept>
#include <cmath>

Matrix::Matrix(){
    dim = 1;
    data.push_back(0.0);
    resolution  = glob_resolution;
}

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

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> args, double a_resolution){
    dim = args.size();
    for(auto row : args){
        if(row.size() != dim){
            throw std::invalid_argument("Initialization list must have same number of columns as dim");
        }
    }
    resolution = a_resolution;
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
// In Place scaling of row
    if( (row < 0) ||  (row >= dim)){
        throw std::invalid_argument("Out of bounds array");
    }
    for(int j=0; j< dim; ++j){
        this->data[get_index(row,j)] *= scaling;
    }
}

Matrix Matrix::Inverse(bool debug){
    // Make a copy...
    Matrix in = *this;
    // and make the output initially the identity
    Matrix out = MatIdentity(dim);
    for(int col=0; col<dim; ++col){
        if(debug){
            std::cout << "Start of loop " << col << std::endl;
            std::cout << in << std::endl;
            std::cout << out << std::endl;
        }
        double pivot = in.data[get_index(col,col)];
        // Check for 0 pivot. If found, we need to swap rows to find a new pivot
        if(is_same(pivot,0)){
            bool new_pivot_found = false;
            // look at rows below current invalid pivot
            for(int row=col+1; row<dim; ++row){
                double new_pivot = in.data[get_index(row,col)];
                // If we found a non-zero pivot
                if(!is_same(new_pivot,0)){
                    // Move new pivot row to cur row
                    in.swap_row(col,row);
                    out.swap_row(col,row);
                    pivot = new_pivot;
                    new_pivot_found = true;
                    break;
                }
            }
            // If no pivots found, this means entire column of matrix if 0(ish). Hence  inverse doesn't exist
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
            // Skip  pivot row
            if(row==col){
                continue;
            }
            // get the element in the same column as pivot element and negate to get scaling factor
            double scaling = -1.0*in.data[get_index(row,col)];
            for(int sweep=0; sweep<dim; ++sweep){
                // Do the row elimination on both copy of initial matrix, and output matrix
                in.data[get_index(row,sweep)] += in.data[get_index(col,sweep)]*scaling;
                out.data[get_index(row,sweep)] += out.data[get_index(col,sweep)]*scaling;
            }
        }
        if(debug){
            std::cout << "Added "  << std::endl;
            std::cout << in << std::endl;
            std::cout << out << std::endl;
        }
    } // Iterated through all rows
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

Matrix MatTranslation(double x, double y, double z){
    return Matrix({{1,0,0,x},{0,1,0,y},{0,0,1,z},{0,0,0,1}});
}

Matrix MatScaling(double x, double y, double z){
    return Matrix({{x,0,0,0},{0,y,0,0},{0,0,z,0},{0,0,0,1}});
}

Matrix MatRotateX(double angle){
    return Matrix({{1,0,0,0},{0, cos(angle), -sin(angle),0},
    {0, sin(angle), cos(angle),0},{0,0,0,1}});
}
Matrix MatRotateY(double angle){
    return Matrix({{cos(angle),0,sin(angle),0},{0,1,0,0},
    {-sin(angle),0,cos(angle),0},{0,0,0,1}});
}
Matrix MatRotateZ(double angle){
    return Matrix({{cos(angle),-sin(angle),0,0},{sin(angle), cos(angle), 0,0}
    ,{0,0,1,0},{0,0,0,1}});
}

Matrix MatShear(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y){
    return Matrix({{1,x_y,x_z,0},{y_x,1,y_z,0},{z_x,z_y,1,0},{0,0,0,1}});
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
