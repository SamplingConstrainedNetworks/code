
#include "sampler.h"


void uniform_sampling(unsigned int blocks) {
    FixedDegreeNetwork network(3, blocks);

    Histogram<unsigned int> histogram(0, network.get_triangles(),
                                      network.get_triangles() + 1);
    Random rng(2);

    UniformSampler sampler(rng, histogram, network);
    sampler.sample(1000000);

    histogram.export_histogram(format("uniform_%d.dat", blocks));
}


void canonic_sampling(unsigned int blocks) {
    FixedDegreeNetwork network(3, blocks);

    Histogram<unsigned int> histogram(0, network.get_triangles(),
                                      network.get_triangles() + 1);
    Random rng(2);

    double beta = 1;
    CanonicSampler sampler(rng, histogram, network, beta);
    sampler.sample(1000000);
    histogram.export_histogram(format("canonic_%d.dat", blocks));
}


void wang_landau(unsigned int blocks) {
    FixedDegreeNetwork network(3, blocks);

    Histogram<unsigned int> histogram(0, network.get_triangles(), network.get_triangles());
    Random rng(2);

    WangLandauSampler sampler(rng, histogram, network);
    sampler.sample(15);

    histogram.export_histogram(format("wl_%d.dat", blocks));
    sampler.export_entropy(format("wl_%d.dat", blocks));
}


int main() {
    char *env_blocks = getenv("BLOCKS");
    if (env_blocks == NULL) {std::cout << "BLOCKS not defined" << std::endl; exit(1);}
    unsigned int blocks = (unsigned int)atoi(env_blocks);

    char *env_method = std::getenv("METHOD");
    if (env_method == NULL) {std::cout << "METHOD not defined" << std::endl; exit(1);}
    std::string method = std::string(env_method);

    if (method == "UniformSampling")
        uniform_sampling(blocks);
    else if (method == "CanonicSampling")
        canonic_sampling(blocks);
    else if (method == "WangLandau")
        wang_landau(blocks);
    else {
        std::cout << "METHOD not valid" << std::endl;
        exit(1);
    }
    return 0;
}
