#include "RNG.h"
#include <random>

void RNG::set_normal(double mu, double sigma){
    nd = std::normal_distribution<double>(mu,sigma);
}

void RNG::set_uniform(double min, double max){
    ud = std::uniform_real_distribution<double>(min,max);
}

double RNG::roll_normal(){
    return nd(rng);
}

double RNG::roll_uniform(){
    return ud(rng);
}
