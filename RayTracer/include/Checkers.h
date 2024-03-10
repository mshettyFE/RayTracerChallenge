#ifndef CHECKERS_H
#define CHECKERS_H

#include "Pattern.h"
#include "Color.h"

class Checkers : public Pattern{
private:
    Color color_a;
    Color color_b;
public:
    Checkers(const Color& a, const Color& b, Matrix trans=MatIdentity(4));
    Color at(const Tuple& pt) const override;
};

#endif