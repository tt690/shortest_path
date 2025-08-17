#include "include/catch.hpp"

#include <vector>
#include <iostream>

#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/graph.hpp"
#include "../src/include/bmssp.hpp"

struct MockGraph {
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    Graph* graph;
    Node *a, *b, *c, *d;
    Edge *ab, *bc, *cd;
    MockGraph() {
        a = new Node("A");
        b = new Node("B");
        c = new Node("C");
        d = new Node("D");
        ab = new Edge(a, b, 1);
        bc = new Edge(b, c, 1);
        cd = new Edge(c, d, 1);
        a->edges.push_back(ab);
        b->edges.push_back(bc);
        c->edges.push_back(cd);
        nodes = {a, b, c, d};
        edges = {ab, bc, cd};
        graph = new Graph(nodes, edges);
    }
    ~MockGraph() {
        delete ab; delete bc; delete cd;
        delete a; delete b; delete c; delete d;
        delete graph;
    }
};

TEST_CASE("BMSSP FindPivots returns expected pivots", "[bmssp]") {
    MockGraph mg;
    size_t degree = 2;
    BMSSP bmssp(*mg.graph, degree);
    std::vector<Node*> S = {mg.a};
    for (Node* node : mg.nodes) node->distanceToSource = INT_MAX;
    mg.a->distanceToSource = 0;
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

    // baseCase test
    for (Node* node : mg.nodes) node->distanceToSource = INT_MAX;
    mg.a->distanceToSource = 0;
    int upperbound = 10;
    auto baseResult = bmssp.baseCase(upperbound, S);
    int boundary = baseResult.first;
    std::vector<Node*> U = baseResult.second;
    // Boundary should be <= upperbound
    REQUIRE(boundary <= upperbound);
    // U should contain at least the source node
    REQUIRE(std::find(U.begin(), U.end(), mg.a) != U.end());
    // U should not be empty
    REQUIRE(!U.empty());
}

TEST_CASE("BMSSP findShortestPath returns correct path", "[bmssp]") {
    MockGraph mg;
    size_t degree = 2;
    BMSSP bmssp(*mg.graph, degree);
    std::vector<Node*> path = bmssp.findShortestPath(mg.a, mg.d);

    // Path should be a -> b -> c -> d
    REQUIRE(path.size() == 4);
    REQUIRE(path[0]->label == "A");
    REQUIRE(path[1]->label == "B");
    REQUIRE(path[2]->label == "C");
    REQUIRE(path[3]->label == "D");
}
