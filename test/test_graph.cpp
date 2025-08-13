#include <vector>
#include <string>

#include "include/catch.hpp"
#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/graph.hpp"

TEST_CASE("Dijkstra finds shortest path and cost", "[graph]") {
	Node* a = new Node("A");
	Node* b = new Node("B");
	Node* c = new Node("C");

	// Create edges (A->B:1, B->C:2, A->C:4)
	Edge* ab = new Edge(a, b, 1);
	Edge* bc = new Edge(b, c, 2);
	Edge* ac = new Edge(a, c, 4);

	a->edges.push_back(ab);
	a->edges.push_back(ac);
	b->edges.push_back(bc);

	std::vector<Node*> nodes = {a, b, c};
	std::vector<Edge*> edges = {ab, bc, ac};
	Graph graph(nodes, edges);

	std::vector<Node*> path = graph.getShortestPath(c);

	REQUIRE(path.size() == 3);
	REQUIRE(path[0]->label == "A");
	REQUIRE(path[1]->label == "B");
	REQUIRE(path[2]->label == "C");

	int cost = graph.getCostOfPath(path);
	REQUIRE(cost == 3);

	delete ab;
	delete bc;
	delete ac;
	delete a;
	delete b;
	delete c;
}