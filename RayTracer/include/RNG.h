#ifndef RNG_H
#define RNG_H

#include <random>

class RNG{
private:
    static inline std::random_device rd;
    static inline std::default_random_engine rng{rd()};
    static inline std::uniform_real_distribution<double> ud{0,1};
    static inline std::normal_distribution<double> nd{0,1};
public:
    void set_normal(double mu, double sigma);
    void set_uniform(double min, double max);
    double roll_normal();
    double roll_uniform();
};


#endif