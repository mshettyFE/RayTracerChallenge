#ifndef GRADIENT_PATTERN_H
#define GRADIENT_PATTERN_H

#include "Pattern.h"
#include "Color.h"
#include "Matrix.h"
#include "Tuple.h"


class GradientPattern : public Pattern{
private:
    Color lower;
    Color upper;
public:
    GradientPattern(Color lower, Color upper, Matrix transform=MatIdentity(4));
    Color at(const Tuple& pt) const override;
};


#endif