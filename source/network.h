#ifndef network_h
#define network_h

#include <cmath>  // for `exp`
#include <assert.h>
#include <vector>
#include <set>
#include <string>
#include <algorithm>


//! A network defined by nodes indexed by 0,1,...,N-1 and a link list.
//! The link list is of the form n_i: {n_j,...,n_k} where n_j...n_k are nodes
//! linked to n_i. Links list contain both link AB and BA.
class Network
{
protected:
    unsigned int Nnodes; //! number of nodes
    std::vector<std::set<unsigned int> > links; //! links list

    unsigned int total_triangles;
    std::vector<unsigned int> triangle_count; //! a cache.

    //! computes the number of triangles that a given node has.
    unsigned int compute_triangles(unsigned int node_i) const {
        unsigned int triangles = 0;
        for (unsigned int node_j : links[node_i])
            for (unsigned int node_k : links[node_i])
                if (node_k != node_j and links[node_j].count(node_k))
                    triangles++;
        return triangles/2;
    }

    //! Uses a brute force loop O(Nnodes^3) to compute the number of triangles:
    //! for every node_i, goes to neiber node_j and checks if neiber of
    //! node_j, node_k, is node_i.
    void compute_triangles() {
        total_triangles = 0;
        for (unsigned int node_i = 0; node_i < Nnodes; node_i++) {
            triangle_count[node_i] = compute_triangles(node_i);
            total_triangles += triangle_count[node_i];
        }
    }

    //! Utility function used to update number of triangles after the network
    //! changed.
    //! If added=true, updates assuming a new link between node_i and node_j.
    //! Else, updates assuming a removed link between node_i and node_j.
    void update_triangles(unsigned int node_i, unsigned int node_j, bool added) {
        int sign = 2*added - 1;

        // update triangles in node_i
        unsigned int removed_triangles_i = 0;
        for (unsigned int node_k : links[node_i])
            if (node_k != node_j and links[node_j].count(node_k))
                removed_triangles_i++;
        triangle_count[node_i] += sign*removed_triangles_i;
        total_triangles += sign*removed_triangles_i;

        // update triangles in node_j
        unsigned int removed_triangles_j = 0;
        for (unsigned int node_k : links[node_j])
            if (node_k != node_i and links[node_i].count(node_k))
                removed_triangles_j++;
        triangle_count[node_j] += sign*removed_triangles_j;
        total_triangles += sign*removed_triangles_j;

        // update triangles in the other nodes.
        std::vector<unsigned int> intersection;
        std::set_intersection(links[node_i].begin(),
                              links[node_i].end(),
                              links[node_j].begin(),
                              links[node_j].end(),
                              std::back_inserter(intersection));

        for (unsigned int node_k : intersection)
            triangle_count[node_k] += sign*1;
        total_triangles += sign*intersection.size();
    }

    //! Checks that link list is consistent: if contains AB then also contains BA.
    void check_consistency() const {
        for (unsigned int node_i = 0; node_i < Nnodes; node_i++)
            for(unsigned int node_j : links[node_i])
                assert(links[node_j].count(node_i) == 1);
    }
public:

    Network(unsigned int Nnodes, std::vector<std::set<unsigned int> > links) :
    Nnodes(Nnodes), links(links), triangle_count(Nnodes) {
        check_consistency();
        compute_triangles();
    }

    //! get number of nodes
    inline unsigned int getN() const {return Nnodes;}

    void print_links() const {
        for (unsigned int node_i = 0; node_i < Nnodes; node_i++) {
            printf("%d: ", node_i);
            for (unsigned int node_j : links[node_i])
            {
                printf("%d ", node_j);
            }
            printf("\n");
        }
    }

    std::set<unsigned int> const& get_links(unsigned int node_i) const {
        return links[node_i];
    }

    unsigned int get_triangles() const {
#ifdef DEBUG
        unsigned int triangles = 0;
        for (unsigned int node_i = 0; node_i < Nnodes; node_i++) {
            triangles += triangle_count[node_i];
        }
        assert(triangles == total_triangles);
#endif
        return total_triangles/3;  // each node counts 3 times on each triangle
    }

    void add_link(unsigned int node_i, unsigned int node_j) {
        assert(links[node_i].count(node_j) == 0);  // link must not exist

        update_triangles(node_i, node_j, true);

        links[node_i].insert(node_j);
        links[node_j].insert(node_i);
    }

    void remove_link(unsigned int node_i, unsigned int node_j) {
        assert(links[node_i].count(node_j) == 1);  // link must exist

        update_triangles(node_i, node_j, false);

        links[node_i].erase(node_j);
        links[node_j].erase(node_i);
    }
};


//! A network initialized from a degree and a number of blocks.
//! It has (degree + 1)*blocks nodes and starts in the configuration with the
//! most number of triangles: (degree + 1)*blocks.
class FixedDegreeNetwork : public Network {
protected:
    static std::vector<std::set<unsigned int> > generate_block_links(unsigned int degree,
                                                                     unsigned int blocks) {
        unsigned int nodes_per_block = degree + 1;
        std::vector<std::set<unsigned int> > links(nodes_per_block*blocks);

        for (unsigned int block_i = 0; block_i < blocks; block_i++)
            for (unsigned int node_i = 0; node_i < nodes_per_block; node_i++)
                for (unsigned int node_j = node_i + 1; node_j < nodes_per_block; node_j++) {
                    links[node_i + nodes_per_block*block_i].insert(node_j + nodes_per_block*block_i);
                    links[node_j + nodes_per_block*block_i].insert(node_i + nodes_per_block*block_i);
                }
        return links;
    }
public:
    FixedDegreeNetwork(unsigned int degree, unsigned int blocks) :
    Network((degree + 1)*blocks, generate_block_links(degree, blocks)) {}
};

#endif
