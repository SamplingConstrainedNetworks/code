# Networks constrained ensembles

This code generates ensembles of N-node networks with a fixed degree sequence
and different clustering coefficients. It uses a flat-histogram Monte Carlo method (based on the Wang-Landau algorithm).

Information about this method can be found in the article 
[Sampling motif-constrained ensembles of networks](http://arxiv.org/abs/1507.08696), by R. Fischer, J. C. Leitão, T. P. Peixoto and E. G. Altmann.
This repository contains two scrips, `fig1.sh` and `fig2.sh`, that demonstrate how to 
reproduce some of the results of the article.

This code was originally written by Rico Fischer with help of [Jorge C. Leitão](http://jorgecarleitao.net), as part of Rico's Diploma Thesis supervised by [Eduardo G. Altmann](http://www.pks.mpg.de/~edugalt/) at the Max Planck Institute for the Physics of Complex Systems in Dresden (Germany) and article above.

The code is written in C++11, has no dependencies and is licenced under MIT.
 
In case you find this code useful, feel free to let us know why and how. Feel free to add issues, suggestions or
questions to the issue tracker of this repository.

## Compiling and running

As a headers only code, you only need to include the `source` directory. 

Examples of usage of this code can be found in `examples`. The `.cpp`s contain the relevant declarations and calls,
the script files compile and run the executable with paramaters (asked by `.cpp`), using cmake and make.

To compile the examples, use:

    g++ -std=c++11 examples/entropy.cpp -Isource -o example
    mkdir fig1_results # <- entropy.cpp outputs the result to this directory 
    BLOCKS=4 ROUND_TRIPS=4 ./example

Alternatively, we also provide a basic CMake project in case your IDE supports cmake.

## Tests

This code contains a test suite in GoogleTest. To run it, you need to compile gtest
in your computer, see e.g. [this](http://stackoverflow.com/a/13513907/931303) stackoverflow answer.

Afterwards, you only need to compile and run it:

    g++ -std=c++11 test/main.cpp -Isource -Itest -Idependencies/include -Ldependencies -lgtest -o tests
    ./tests
    # 1. include the source: -Isource
    # 2. include the tests: -Itest
    # 3. include the gtest headers: -Idependencies/include (location of include/gtest)
    # 4. set library location: -Ldependencies (location of libgtest.so, libgtest.a, or gtest.framework); 
    # 5. use gtest library: -lgtest
