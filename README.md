# Triangles in networks

This code generates an ensemble of networks with a fixed number of
nodes, a fixed node-degree, and a fixed average number of triangles.

## Installation and running

1. Install [GSL](http://www.gnu.org/software/gsl/)
2. clone the repository.
3. within the project, create directories `release/results/` and
   `build/`
4. run `make`. If something fail, change the compiler flags
   `CINCLUDE` and `CFLAGS` in `Makefile` to correctly find GSL.
5. `cd release`
6. run the code:

`cd ..; make; cd release/; ./MainProgram`

## Running the test suite

This code contains a test suite in GoogleTest. To run it, you need
to

1. install gtest, see http://stackoverflow.com/a/13513907/931303
2. `mkdir test_build; mkdir test_release;`
3. `make test`
4. `cd test_release; ./TestProgram`
