#include <gmock/gmock.h>

MATCHER_P(TupleNear, expected, "") {
    double epsilon = 0.00001;
    return std::abs(arg.x() - expected.x()) < epsilon &&
           std::abs(arg.y() - expected.y()) < epsilon &&
           std::abs(arg.z() - expected.z()) < epsilon &&
           std::abs(arg.w() - expected.w()) < epsilon;
}