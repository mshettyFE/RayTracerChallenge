#ifndef PATTERN_H
#define PATTERN_H

#include "Matrix.h"
#include "Color.h"
#include "Shape.h"
#include <memory>

class Pattern{
protected:
    Matrix transformation;
public:
    Pattern(Matrix trans=MatIdentity(4));
    virtual Color at(const Tuple& pt) const = 0;
    virtual Color at_object(const Tuple& pt, const std::shared_ptr<Shape> obj) const = 0;
    void set_matrix(const Matrix& new_mat);
};

#endif