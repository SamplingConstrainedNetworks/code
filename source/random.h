#ifndef triangles_random_h
#define triangles_random_h

#include <random>


//! A class implementation for RNG of normal and uniform distributions.
class Random {
protected:
    unsigned int seed;	//!< seed of the rng
    std::random_device rd;
    std::mt19937 generator;
    std::normal_distribution<> normal;
    std::uniform_real_distribution<> uniform;
public:
    //! Default constructor: uses a random seed
    Random() : seed(std::random_device{}()), generator(seed) {}
    //! Constructor: uses argument as the seed
    Random(unsigned int seed) : seed(seed), generator(seed) {};

    unsigned int get_seed() const {
        return seed;
    };

    //! returns an integer random number under uniform distribution on interval \f$[0,N[\f$
    unsigned int R(unsigned int min, unsigned int max) {
        return std::uniform_int_distribution<unsigned int>{min, max - 1}(generator);
    }
    //! returns a real random number under uniform distribution on interval \f$[0,1]\f$
    long double R() {
        return uniform(generator);
    }
    //! returns a real random number from a normal distribution on interval \f$[-\infty,\infty]\f$
    long double normalR() {
        return normal(generator);
    }
};

#endif
