#include <vector>
#include <iostream>
#include "include/catch.hpp"

#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/graph.hpp"
#include "../src/include/bmssp.hpp"

TEST_CASE("BMSSP FindPivots returns expected pivots", "[bmssp]") {
    // Create nodes
    Node* a = new Node("A");
    Node* b = new Node("B");
    Node* c = new Node("C");
    Node* d = new Node("D");

    // Create edges
    Edge* ab = new Edge(a, b, 1);
    Edge* bc = new Edge(b, c, 1);
    Edge* cd = new Edge(c, d, 1);
    a->edges.push_back(ab);
    b->edges.push_back(bc);
    c->edges.push_back(cd);

    std::vector<Node*> nodes = {a, b, c, d};
    std::vector<Edge*> edges = {ab, bc, cd};
    Graph graph(nodes, edges);

    // BMSSP setup
    size_t degree = 2;
    BMSSP bmssp(graph, degree);
    std::vector<Node*> S = {a};
    
    // Set initial distances
    for (Node* node : nodes) node->distanceToSource = INT_MAX;
    a->distanceToSource = 0;

    auto result = bmssp.findNodes(INT_MAX, S);
    std::vector<Node*> pivots = result.first;
    std::vector<Node*> weight = result.second;

    // Test: pivots should not be empty
    REQUIRE(!pivots.empty());
    // Test: pivots should be subset of S
    for (Node* p : pivots) {
        REQUIRE(std::find(S.begin(), S.end(), p) != S.end());
    }
    // Test: weight should contain at least the input nodes
    for (Node* s : S) {
        REQUIRE(std::find(weight.begin(), weight.end(), s) != weight.end());
    }

    SECTION("BMSSP baseCase returns correct boundary and set") {
        // Reset distances
        for (Node* node : nodes) node->distanceToSource = INT_MAX;
        a->distanceToSource = 0;
        int upperbound = 10;
        auto baseResult = bmssp.baseCase(upperbound, S);
        int boundary = baseResult.first;
        std::vector<Node*> U = baseResult.second;
        // Boundary should be <= upperbound
        REQUIRE(boundary <= upperbound);
        // U should contain at least the source node
        REQUIRE(std::find(U.begin(), U.end(), a) != U.end());
        // U should not be empty
        REQUIRE(!U.empty());
    }

    SECTION("BMSSP findShortestPath returns correct path") {
        // Reset distances
        for (Node* node : nodes) node->distanceToSource = INT_MAX;
        a->distanceToSource = 0;
        std::vector<Node*> path = bmssp.findShortestPath(a, d);
        // Path should be a -> b -> c -> d
        REQUIRE(path.size() == 4);
        REQUIRE(path[0]->label == "A");
        REQUIRE(path[1]->label == "B");
        REQUIRE(path[2]->label == "C");
        REQUIRE(path[3]->label == "D");
    }

    // Clean up
    delete ab;
    delete bc;
    delete cd;
    delete a;
    delete b;
    delete c;
    delete d;
}
