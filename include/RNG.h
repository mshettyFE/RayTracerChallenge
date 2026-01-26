#ifndef RNG_H
#define RNG_H

#include <random>

class RNG{
private:
    static inline std::default_random_engine engine= std::default_random_engine(time(0));;
    static inline  std::uniform_real_distribution<double> udd {};
    static inline  std::normal_distribution<double> nd {};
public:
    double inline  roll_uniform(double low, double high)
    {
        return udd(engine, std::uniform_real_distribution<double>::param_type{low,high});
    }

    double inline  roll_normal(double mean, double sigma)
    {
        return nd(engine, std::normal_distribution<double>::param_type{mean,sigma});
    }
};


#endif