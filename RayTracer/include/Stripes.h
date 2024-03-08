#ifndef STRIPES_H
#define STRIPES_H

#include "Pattern.h"
#include "Color.h"

class Stripes : public Pattern{
private:
    Color color_a;
    Color color_b;
public:
    Stripes(const Color& a, const Color& b, Matrix trans=MatIdentity(4));
    Color at(const Tuple& pt) const override;
};

#endif