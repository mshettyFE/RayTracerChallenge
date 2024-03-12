#ifndef RING_PATTERN_H
#define RING_PATTERN_H

#include "Pattern.h"
#include "Color.h"
#include "Matrix.h"

class RingPattern : public Pattern{
private:
    Color a;
    Color b;
public:
    RingPattern(Color a, Color b,Matrix trans=MatIdentity(4));
    Color at(const Tuple& pt) const override;
};

#endif