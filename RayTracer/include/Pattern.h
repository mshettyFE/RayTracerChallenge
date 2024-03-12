#ifndef PATTERN_H
#define PATTERN_H

#include "Matrix.h"
#include "Color.h"
#include <memory>

class Shape;

class Pattern{
private:
    Matrix transformation;
 protected:
    std::string name;
public:
    Pattern(Matrix trans=MatIdentity(4), std::string name="PLACEHOLDER");
    virtual Color at(const Tuple& pt) const = 0;

    Color at_object(const Tuple& pt, const std::shared_ptr<Shape>& obj) const;

    void set_transformation(const Matrix& new_mat);
    Matrix get_transformation() const;

    std::string get_name() const;

};

#endif