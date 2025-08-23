#include "include/catch.hpp"

#include <vector>
#include <iostream>

#include "include/mock_graphs.hpp"
#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/graph.hpp"
#include "../src/include/bmssp.hpp"

TEST_CASE("BMSSP find_shortest_path returns correct path", "[bmssp]") {
    MockGraph mg;
    size_t degree = 2;
    BMSSP bmssp(*mg.graph, degree);
    bmssp.find_shortest_path(mg.a, mg.d);
    std::vector<Node*> path = bmssp.reconstruct_path(mg.a, mg.d);

    REQUIRE(path.size() == 4);
    REQUIRE(path[0]->label == "A");
    REQUIRE(path[1]->label == "B");
    REQUIRE(path[2]->label == "C");
    REQUIRE(path[3]->label == "D");

	int weight = mg.graph->getCostOfPath(path);
	REQUIRE(weight == 3);
}

TEST_CASE("BMSSP find_shortest_path returns correct path on complex graph", "[bmssp]") {
    ComplexMockGraph mg;
    size_t degree = 3;
    BMSSP bmssp(*mg.graph, degree);
    bmssp.find_shortest_path(mg.a, mg.g);
    std::vector<Node*> path = bmssp.reconstruct_path(mg.a, mg.g);

    REQUIRE(path.size() == 7);
    REQUIRE(path[0]->label == "A");
    REQUIRE(path[1]->label == "B");
    REQUIRE(path[2]->label == "C");
    REQUIRE(path[3]->label == "D");
    REQUIRE(path[4]->label == "E");
    REQUIRE(path[5]->label == "F");
    REQUIRE(path[6]->label == "G");

    int weight = mg.graph->getCostOfPath(path);
	REQUIRE(weight == 10);
}