#ifndef triangles_proposal_h
#define triangles_proposal_h

#include "network.h"
#include "random.h"


typedef std::pair<unsigned int, unsigned int> Link;


//! a proposal of FixedDegree is identified by 4 links:
//! 2 old links that will be removed and 2 new links that will be added.
struct GeneratedProposal {
    Link old_link1;
    Link old_link2;
    Link new_link1;
    Link new_link2;
};


//! Switches two links constrained to maintain a fixed degree on all nodes.
//! 1. Picks an existing random link "AB"
//! 2. Generates a new link "AC"
//! 3. Picks an existing random link "CD"
//! 4. Generates the new link "DB"
class FixedDegreeProposer {
protected:
    Random & rng;

    //! 1. Picks an existing random link "AB"
    Link random_old_link(Network const& network) const {
        Link link;

        link.first = rng.R(0, network.getN());

        std::set<unsigned int> const& list = network.get_links(link.first);
        assert(list.size() != 0);

        // generates a random neighberhood, link.second, of link.first
        unsigned int index_j = rng.R(0, list.size());
        std::set<unsigned int>::const_iterator it = list.begin();
        std::advance(it, index_j);
        link.second = *it;

        return link;
    }

    //! 2. Generates a new random link "AC"
    //! ensures that "C != A and C is not neighberhood of A"
    Link random_new_link(Network const& network, Link old_link) const {
        Link new_link(old_link);

        std::set<unsigned int> const& list = network.get_links(new_link.first);
        while (list.count(new_link.second) != 0 or new_link.second == new_link.first) {
            new_link.second = rng.R(0, network.getN());
        }

        return new_link;
    }

    //! 3. Picks an existing random link "CD"
    //! ensures that "D != B and B is not a neighberhood of D" since otherwise
    //! `new_link(...)` would generate an existing link.
    Link old_link(Network const& network, Link new_link1, Link old_link1) const {
        Link old_link2;
        old_link2.first = new_link1.second;
        old_link2.second = new_link1.first;

        std::set<unsigned int> const& list = network.get_links(old_link2.first);

        while (network.get_links(old_link1.second).count(old_link2.second) != 0 or
               old_link2.second == old_link1.second) {
            // generate a random neighberhood, old_link2.second, of old_link2.first
            unsigned int index_j = rng.R(0, list.size());
            std::set<unsigned int>::const_iterator it = list.begin();
            std::advance(it, index_j);
            old_link2.second = *it;
        }

        return old_link2;
    }

    //! 4. Generates the new link "DB"
    Link new_link(Link old_link1, Link old_link2) const {
        Link new_link2;

        new_link2.first = old_link2.second;
        new_link2.second = old_link1.second;

        return new_link2;
    }
public:

    FixedDegreeProposer(Random & rng) : rng(rng) {}

    //! generates a valid proposal
    GeneratedProposal generate_proposal(Network const& network) const {
        GeneratedProposal result;

        result.old_link1 = random_old_link(network);
        result.new_link1 = random_new_link(network, result.old_link1);
        result.old_link2 = old_link(network, result.new_link1, result.old_link1);
        result.new_link2 = new_link(result.old_link1, result.old_link2);

        return result;
    }

    //! Applies the proposal to the network
    void propose(Network & network, GeneratedProposal const& result) const {
        network.remove_link(result.old_link1.first, result.old_link1.second);
        network.remove_link(result.old_link2.first, result.old_link2.second);
        network.add_link(result.new_link1.first, result.new_link1.second);
        network.add_link(result.new_link2.first, result.new_link2.second);
    }

    //! Uses the proposal to rollback the network to the state it was before
    //! that proposal was applied (inverse of `propose`)
    void rollback(Network & network, GeneratedProposal const& result) const {
        network.add_link(result.old_link1.first, result.old_link1.second);
        network.add_link(result.old_link2.first, result.old_link2.second);
        network.remove_link(result.new_link1.first, result.new_link1.second);
        network.remove_link(result.new_link2.first, result.new_link2.second);
    }

    //! Utility method that generates the proposal and automatically applies it.
    void propose(Network & network) const {
        GeneratedProposal result = generate_proposal(network);
        propose(network, result);
        check_degree_consistency(network);
    }

    //! Asserts that degree is the same on all nodes.
    static void check_degree_consistency(Network const& network) {
        unsigned int node_0_degree = (unsigned int)network.get_links(0).size();
        for (unsigned int node_i = 1; node_i < network.getN(); node_i++) {
            unsigned int node_i_degree = (unsigned int)network.get_links(node_i).size();
            assert(node_i_degree == node_0_degree);
        }
    }
};

#endif
