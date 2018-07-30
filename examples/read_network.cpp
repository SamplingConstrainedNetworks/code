// git@github.com:SamplingConstrainedNetworks/code.git
// g++ -std=c++11 examples/read_network.cpp -Isource -o read_network.exe
// ./read_network.exe
#include <vector>
#include <iostream>
#include <set>

#include "sampler.h"
#include "io.h"

std::vector<std::vector<unsigned int>> network_to_list(Network const& network) {
    std::vector<std::vector<unsigned int>> result;

    for (unsigned int node_i = 0; node_i < network.getN(); node_i++) {
        for (unsigned int node_j : network.get_links(node_i))
            result.push_back({node_i, node_j});
    }
    return result;
};


int main() {
    // the network to load
    Network network("./examples/input_network.dat");

    Histogram<unsigned int> histogram(0, network.get_triangles(), network.get_triangles());

    Random rng(2);  // the number is the seed

    unsigned int target_triangles = network.get_triangles()/2;
    unsigned int target_networks = 10;
    std::cout << "triangles: " << network.get_triangles() << std::endl;
    std::cout << "target triangles: " << target_triangles << std::endl;

    // this is the sampler: how we move in the triangle space
    // this sampler preserves the degree sequence because proposals preserve the degree sequence (uses FixedDegreeProposer)
    double beta = 1.0;
    CanonicSampler sampler(rng, histogram, network, beta);

    // warm up
    std::cout << "warm-up started" << std::endl;
    for (unsigned int step = 0; step < 1000; step++)
        sampler.markov_step();
    std::cout << "warm-up finished" << std::endl;

    // output networks
    unsigned int found_networks = 0;
    while (found_networks < target_networks) {
        sampler.markov_step();
        if (network.get_triangles() > target_triangles - 1 and network.get_triangles() < target_triangles + 1) {
            // network with target_triangles found. Output it to a file
            found_networks++;

            std::string file_name = format("./output/network_%d.dat", found_networks);
            io::save(network_to_list(network), file_name);
        };
    };
    return 0;
}
