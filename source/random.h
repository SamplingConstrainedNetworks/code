#ifndef random_h
#define random_h

#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


//! A class implementation of the gsl random number generator (rng).
class Random {
private:
    long int seed;	//!< seed of the rng
    gsl_rng const*const r; //!< pointer to the gsl rng
public:
    //! Default constructor: uses current time as seed
    Random() : seed(time(NULL)), r(gsl_rng_alloc(gsl_rng_taus)) {
        gsl_rng_set(r, seed); // set seed.
    }
    //! Constructor: uses argument as the seed
    Random(long int seed) : seed(seed), r(gsl_rng_alloc(gsl_rng_taus)) {
        gsl_rng_set(r, seed); // set seed.
    };

    long int get_seed() const {
        return seed;
    };

    //! returns an integer random number under uniform distribution on interval \f$[0,N]\f$
    long int R(long int N) {
        return gsl_rng_uniform_int(r, N);
    }
    //! returns a real random number under uniform distribution on interval \f$[0,1]\f$
    long double R() {
        return gsl_rng_uniform_pos(r);
    }
    //! returns a real random number from a normal distribution on interval \f$[-\infty,\infty]\f$
    long double normalR() {
        return gsl_ran_gaussian(r, 1);
    }
};

#endif
