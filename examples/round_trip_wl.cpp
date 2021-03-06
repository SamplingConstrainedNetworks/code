/*
 This code reproduces the main panel of figure 2.
 See fig2.sh on how to compile and run it.

 - The network size is 4*BLOCKS;
 - Increasing ROUND_TRIPS improves the convergence of Wang-Landau;

 Each run of this code is a single point and respective error bars on figure 2.
 The curve in the figure is constructed by picking the average round-trip of the last WL step of each run and plot it
 against the network size (post-process using the result of this code).
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

        double mean_round_trip = 0;
        double std_round_trip = 0;
        for (unsigned int round_trip = 0; round_trip < round_trips; round_trip++) {
            unsigned int previous_count = histogram.count();
            sampler.perform_round_trip();
            unsigned int round_trip_time = histogram.count() - previous_count;

            // update formula: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
            int n = round_trip + 1;
            double delta = round_trip_time - mean_round_trip;
            mean_round_trip += delta/n;
            std_round_trip += delta*(round_trip_time - mean_round_trip);
        }
        sampler.wang_landau_step();

        sampler.export_entropy(format("wl_B%d_S%d.dat", blocks, round_trips));
        histogram.export_histogram(format("results/histogram_wl_B%d_S%d.dat", blocks, round_trips));
    }
    return 0;
}
