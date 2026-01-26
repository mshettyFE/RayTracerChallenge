#include "Pattern.h"
#include "RingPattern.h"
#include <cmath>

RingPattern::RingPattern(Color a, Color b,Matrix trans) : Pattern(trans,"Ring") {
    this->a = a;
    this->b = b;
}

Color RingPattern::at(const Tuple& pt) const {
    double check  =std::sqrt(pt[0]*pt[0]+pt[2]*pt[2]);
    if( static_cast<int>(floor(check))%2 == 0){
        return a;
    }
    return b;
}
