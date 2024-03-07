#include "Pattern.h"
#include "Matrix.h"

Pattern::Pattern(Matrix trans){
    transformation = trans;
}

void Pattern::set_matrix(const Matrix& new_mat){
    transformation = new_mat;
}