#ifndef triangles_sampler_h
#define triangles_sampler_h

#include "histogram.h"
#include "network.h"
#include "proposer.h"
#include "io.h"


//! A generic sampler.
class Sampler {
protected:
    Random & rng;
    Histogram<unsigned int> & histogram;

public:
    Sampler(Random & rng,
            Histogram<unsigned int> & histogram) : rng(rng), histogram(histogram) {}
};


//! Sampler that draws samples networks without weights
class UniformSampler : public Sampler {
protected:
    FixedDegreeProposer proposer;
    FixedDegreeNetwork & network;
public:
    UniformSampler(Random & rng,
                   Histogram<unsigned int> & histogram,
                   FixedDegreeNetwork & network) :
    Sampler(rng, histogram), proposer(rng), network(network) {}

    void sample(unsigned int total_samples) {
        // burn time: go to most probable network
        while (network.get_triangles() != 0)
            markov_step();
        for (unsigned int sample = 0; sample < total_samples; sample++) {
            markov_step();
            histogram.add(network.get_triangles());
        }
    }

    virtual void markov_step() {
        proposer.propose(network);
    }
};


//! Sampler that draws samples from the canonic distribution on the number of triangles
class CanonicSampler : public UniformSampler {
protected:
    double beta;
public:
    CanonicSampler(Random & rng,
                   Histogram<unsigned int> & histogram,
                   FixedDegreeNetwork & network, double beta) :
    UniformSampler(rng, histogram, network), beta(beta) {}

    void markov_step() {
        unsigned int old_triangles = network.get_triangles();

        GeneratedProposal proposal = proposer.generate_proposal(network);
        proposer.propose(network, proposal);

        unsigned int new_triangles = network.get_triangles();

        bool was_accepted = true;
        // if rejected
        if (rng.R() > exp(beta*(new_triangles - old_triangles))) {
            proposer.rollback(network, proposal);
            was_accepted = false;
        }

        histogram.add(old_triangles);
    }

    void sample(unsigned int total_samples) {
        // burn time: go to most probable network
        while (network.get_triangles() != 0)
            markov_step();

        // Measure
        histogram.reset();
        for (unsigned int sample = 0; sample < total_samples; sample++) {
            markov_step();
        }
    }
};


//! Sampler that computes the DOS of number of triangles using Wang-Landau algorithm
class WangLandauSampler : public UniformSampler {
    std::vector<double> entropy;
    double f;
public:
    WangLandauSampler(Random & rng,
                      Histogram<unsigned int> & histogram,
                      FixedDegreeNetwork & network) :
    UniformSampler(rng, histogram, network), entropy(histogram.bins() + 1), f(1) {}

    void markov_step() {
        unsigned int old_triangles = network.get_triangles();

        GeneratedProposal proposal = proposer.generate_proposal(network);
        proposer.propose(network, proposal);

        unsigned int new_triangles = network.get_triangles();

        bool was_accepted = true;
        // if rejected
        if (rng.R() > exp(entropy[old_triangles] - entropy[new_triangles])) {
            proposer.rollback(network, proposal);
            was_accepted = false;
        }

        histogram.add(network.get_triangles());
        entropy[histogram.bin(network.get_triangles())] += f;
    }

    inline void wang_landau_step() {
        f /= 2;
    }

    //! performs a round-trip.
    void perform_round_trip() {
        bool going_up = true;

        while (true) {
            markov_step();
            unsigned int triangles = network.get_triangles();

            // round-trip control
            if (histogram.bin(triangles) == histogram.bins()
                and going_up) {
                going_up = false;
                //std::cout << "reached up\n";
            }
            else if (histogram.bin(triangles) == 0
                     and not going_up) {
                //std::cout << "reached down\n";
                break;
            }
        }
    }

    //! exports the normalized entropy: \sum(exp(S)) == 1
    void export_entropy(std::string file_name) const {
        std::vector<std::vector<double> > data;

        // sum(exp(S))*exp(C) = 1
        // S_max == max{S}
        // sum(exp(S))*exp(C) = sum(exp(S - S_max))*exp(S_max)*exp(C) = 1
        // C == S_max + log(sum(exp(S - S_max)))
        double S_max = -100;
        for (unsigned int b = 0; b <= histogram.bins(); b++)
            if (entropy[b] > S_max)
                S_max = entropy[b];

        double C = 0;
        for (unsigned int b = 0; b <= histogram.bins(); b++)
            C += exp(entropy[b] - S_max);
        C = S_max + log(C);

        for (unsigned int b = 0; b <= histogram.bins(); b++) {
            if (histogram[b] > 0) {
                std::vector<double> row(2);
                row[0] = b;
                row[1] = entropy[b] - C;
                data.push_back(row);
            }
        }
        io::save(data, file_name);
    }

    void sample(unsigned int total_steps, unsigned int round_trips=5) {
        for (unsigned int step = 0; step < total_steps; step++) {
            std::cout << "w-l step: " << step + 1 << "/" << total_steps << std::endl;
            histogram.reset();
            for (unsigned int round_trip = 0; round_trip < round_trips; round_trip++)
                perform_round_trip();
            wang_landau_step();

            histogram.export_histogram("tmp.dat");
            export_entropy("entropy_tmp.dat");
            }
    }
};

#endif
