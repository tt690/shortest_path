#include "include/catch.hpp"

#include <vector>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/edge.hpp"
#include "../src/include/dijkstra.hpp"

TEST_CASE("Dijkstra finds shortest path and weight", "[graph]") {
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
	Dijkstra dijkstra(graph);

	std::vector<Node*> path = dijkstra.findShortestPath(a, c);

	REQUIRE(path.size() == 3);
	REQUIRE(path[0]->label == "A");
	REQUIRE(path[1]->label == "B");
	REQUIRE(path[2]->label == "C");

	int weight = graph.getCostOfPath(path);
	REQUIRE(weight == 3);

	delete ab;
	delete bc;
	delete ac;
	delete a;
	delete b;
	delete c;
}