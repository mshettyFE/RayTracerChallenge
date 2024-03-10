#ifndef TEST_PATTERN_H
#define TEST_PATTERN_H

#include "Pattern.h"
#include "Color.h"

class TestPattern : public Pattern{
private:
    Color color_a;
    Color color_b;
public:
    TestPattern(Matrix trans=MatIdentity(4));
    Color at(const Tuple& pt) const override;
};

#endif