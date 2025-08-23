#include "include/catch.hpp"

#include <vector>
#include <string>

#include "include/mock_graphs.hpp"
#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/dijkstra.hpp"

TEST_CASE("Dijkstra finds shortest path and weight", "[graph]") {
	MockGraph mg;
	Dijkstra dijkstra(*mg.graph);
	Node* start = mg.a;
	Node* end = mg.d;
	std::vector<Node*> path = dijkstra.findShortestPath(start, end);

	REQUIRE(path.size() == 4);
	REQUIRE(path[0]->label == "A");
	REQUIRE(path[1]->label == "B");
	REQUIRE(path[2]->label == "C");
	REQUIRE(path[3]->label == "D");

	int weight = mg.graph->getCostOfPath(path);
	REQUIRE(weight == 3);
}

TEST_CASE("Dijkstra finds shortest path in complex graph", "[graph][complex]") {
	ComplexMockGraph mg;
	Dijkstra dijkstra(*mg.graph);
	Node* start = mg.a;
	Node* end = mg.g;
	std::vector<Node*> path = dijkstra.findShortestPath(start, end);

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