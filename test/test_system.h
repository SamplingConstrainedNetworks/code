#ifndef test_system_h
#define test_system_h

#include "gtest/gtest.h"
#include "network.h"
#include "proposer.h"


class TestTriangle : public ::testing::Test {
public:
    Network * network;

    void SetUp() {
        std::vector<std::set<unsigned int> > links(3);

        links[0].insert(1);
        links[1].insert(0);
        links[0].insert(2);
        links[2].insert(0);
        links[1].insert(2);
        links[2].insert(1);
        network = new Network(3, links);
    }

    void TearDown() {
        delete network;
    }
};


TEST_F(TestTriangle, first) {
    EXPECT_EQ(1, network->get_triangles());
}


class TestIncompleteTriangle : public ::testing::Test {
public:
    Network * network;

    void SetUp() {
        std::vector<std::set<unsigned int> > links(3);

        links[0].insert(1);
        links[1].insert(0);
        links[0].insert(2);
        links[2].insert(0);
        network = new Network(3, links);
    }

    void TearDown() {
        delete network;
    }
};


TEST_F(TestIncompleteTriangle, first) {
    EXPECT_EQ(0, network->get_triangles());
}


class TestSquare : public ::testing::Test {
public:
    Network * network;
    
    void SetUp() {
        std::vector<std::set<unsigned int> > links(4);

        links[0].insert(1);
        links[1].insert(0);
        links[0].insert(2);
        links[2].insert(0);
        links[0].insert(3);
        links[3].insert(0);
        links[1].insert(2);
        links[2].insert(1);
        links[1].insert(3);
        links[3].insert(1);
        links[2].insert(3);
        links[3].insert(2);
        network = new Network(4, links);
    }

    void TearDown() {
        delete network;
    }
};


TEST_F(TestSquare, first) {
    EXPECT_EQ(4, network->get_triangles());

    network->remove_link(0, 1);

    EXPECT_EQ(2, network->get_triangles());

    network->remove_link(0, 2);

    EXPECT_EQ(1, network->get_triangles());

    network->add_link(0, 2);

    EXPECT_EQ(2, network->get_triangles());

    network->add_link(0, 1);

    EXPECT_EQ(4, network->get_triangles());
}


class TestMultiSquare : public ::testing::Test {
public:
    Network * network;

    void SetUp() {
        unsigned int squares = 2;

        std::vector<std::set<unsigned int> > links(4*squares);

        for (unsigned int square_i = 0; square_i < squares; square_i++) {
            links[0 + 4*square_i].insert(1 + 4*square_i);
            links[1 + 4*square_i].insert(0 + 4*square_i);
            links[0 + 4*square_i].insert(2 + 4*square_i);
            links[2 + 4*square_i].insert(0 + 4*square_i);
            links[0 + 4*square_i].insert(3 + 4*square_i);
            links[3 + 4*square_i].insert(0 + 4*square_i);
            links[1 + 4*square_i].insert(2 + 4*square_i);
            links[2 + 4*square_i].insert(1 + 4*square_i);
            links[1 + 4*square_i].insert(3 + 4*square_i);
            links[3 + 4*square_i].insert(1 + 4*square_i);
            links[2 + 4*square_i].insert(3 + 4*square_i);
            links[3 + 4*square_i].insert(2 + 4*square_i);
        }
        network = new Network(4*squares, links);
    }
    
    void TearDown() {
        delete network;
    }
};


TEST_F(TestMultiSquare, constructor) {
    FixedDegreeNetwork network1(3, 2);

    ASSERT_EQ(network->getN(), network1.getN());

    ASSERT_EQ(network->get_triangles(), network1.get_triangles());

    for (unsigned int node_i = 0; node_i < 8; node_i++) {
        EXPECT_EQ(network->get_links(node_i), network1.get_links(node_i));
    }
}


TEST_F(TestMultiSquare, triangles) {
    ASSERT_EQ(8, network->get_triangles());
}


TEST_F(TestMultiSquare, degree) {
    Random rng(1);
    FixedDegreeProposer proposer(rng);

    proposer.check_degree_consistency(network);

    for (unsigned int i = 0; i < 100; i++) {
        GeneratedProposal proposal = proposer.generate_proposal(network);

        /*
        std::cout << "old_link1 "<< proposal.old_link1.first << proposal.old_link1.second << std::endl;
        std::cout << "new_link1 "<< proposal.new_link1.first << proposal.new_link1.second << std::endl;
        std::cout << "old_link2 "<< proposal.old_link2.first << proposal.old_link2.second << std::endl;
        std::cout << "new_link2 "<< proposal.new_link2.first << proposal.new_link2.second << std::endl;
         */

        proposer.propose(network, proposal);

        /*
        std::cout << std::endl;
        network->print_links();
        std::cout << std::endl;
        */
        proposer.check_degree_consistency(network);
    }
}

#endif
