#include "sampler.h"

int main() {
    char *env_blocks = getenv("BLOCKS");
    if (env_blocks == NULL) {std::cout << "BLOCKS not defined" << std::endl; exit(1);}
    unsigned int blocks = (unsigned int)atoi(env_blocks);

    char *env_round_trips = getenv("ROUND_TRIPS");
    if (env_round_trips == NULL) {std::cout << "ROUND_TRIPS not defined" << std::endl; exit(1);}
    unsigned int round_trips = (unsigned int)atoi(env_round_trips);

    unsigned int total_steps = 15;

    FixedDegreeNetwork network(3, blocks);

    Histogram<unsigned int> histogram(0, network.get_triangles(), network.get_triangles());
    Random rng(2);

    WangLandauSampler sampler(rng, histogram, network);

    // warm up
    while (network.get_triangles() != 0)
        sampler.markov_step();

    std::vector<std::vector<double> > result;

    for (unsigned int step = 0; step < total_steps; step++) {
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

        // nodes,MEAN,STD,samples,wl-step
        std::vector<double> temp(5);
        temp[0] = blocks*4;
        temp[1] = mean_round_trip;
        temp[2] = sqrt(std_round_trip/(round_trips - 1));
        temp[3] = round_trips;
        temp[4] = step + 1;
        result.push_back(temp);
    }

    io::save(result, format("results/wl_B%d_S%d.dat", blocks, round_trips));
    return 0;
}
