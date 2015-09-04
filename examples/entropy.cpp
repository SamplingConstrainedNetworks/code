/*
 This code reproduces the WL curve in figure 1 of the paper (see README).
 See fig1.sh on how to compile and run it.

 - The network size is 4*BLOCKS;
 - Increasing ROUND_TRIPS improves the convergence of Wang-Landau;

 Each run of this code is a single WL simulation. After it finished, the output
 is an approximation of the DOS and histogram.

 The curve in figure b) of the figure is \rho(\beta, t) = exp(\beta*t + S(t))/Z(\beta) where S(t) is the entropy outputted
 by this program.
 The curve in figure a) is c(\beta) = sum_t t*\rho(\beta, t)

 The histogram outputted by this program becomes flat as ROUND_TRIPS -> \infty.

 The dependency of this procedure with BLOCKS and ROUND_TRIPS is illustrated in Fig.2.
*/

#include "sampler.h"

int main() {
    char *env_blocks = getenv("BLOCKS");
    if (env_blocks == NULL) {std::cout << "BLOCKS not defined" << std::endl; exit(1);}
    unsigned int blocks = (unsigned int)atoi(env_blocks);

    char *env_round_trips = getenv("ROUND_TRIPS");
    if (env_round_trips == NULL) {std::cout << "ROUND_TRIPS not defined" << std::endl; exit(1);}
    unsigned int round_trips = (unsigned int)atoi(env_round_trips);

    unsigned int total_wl_steps = 15;

    FixedDegreeNetwork network(3, blocks);

    Histogram<unsigned int> histogram(0, network.get_triangles(), network.get_triangles());
    Random rng(2);

    WangLandauSampler sampler(rng, histogram, network);

    // warm up
    while (network.get_triangles() != 0)
        sampler.markov_step();

    std::vector<std::vector<double> > result;

    for (unsigned int step = 0; step < total_wl_steps; step++) {
        histogram.reset();

        for (unsigned int round_trip = 0; round_trip < round_trips; round_trip++) {
            sampler.perform_round_trip();
        }
        sampler.wang_landau_step();

        sampler.export_entropy(format("fig1_results/entropy_B%d_S%d.dat", blocks, round_trips));
        histogram.export_histogram(format("fig1_results/histogram_wl_B%d_S%d.dat", blocks, round_trips));
    }
    return 0;
}
