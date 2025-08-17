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

TEST_CASE("Dijkstra handles cycles and multiple shortest paths", "[graph][complex]") {
	// Graph: A->B:1, B->C:2, C->A:1, A->D:2, D->C:1, B->D:2
	Node* a = new Node("A");
	Node* b = new Node("B");
	Node* c = new Node("C");
	Node* d = new Node("D");

	Edge* ab = new Edge(a, b, 1);
	Edge* bc = new Edge(b, c, 2);
	Edge* ca = new Edge(c, a, 1);
	Edge* ad = new Edge(a, d, 2);
	Edge* dc = new Edge(d, c, 1);
	Edge* bd = new Edge(b, d, 2);

	a->edges.push_back(ab);
	a->edges.push_back(ad);
	b->edges.push_back(bc);
	b->edges.push_back(bd);
	c->edges.push_back(ca);
	d->edges.push_back(dc);

	std::vector<Node*> nodes = {a, b, c, d};
	std::vector<Edge*> edges = {ab, bc, ca, ad, dc, bd};
	Graph graph(nodes, edges);
	Dijkstra dijkstra(graph);

	// Shortest path from A to C: A->B->C (cost 3) or A->D->C (cost 3)
	std::vector<Node*> path = dijkstra.findShortestPath(a, c);
	REQUIRE(path.size() == 3);
	REQUIRE(path[0]->label == "A");
	REQUIRE(path[2]->label == "C");
	int weight = graph.getCostOfPath(path);
	REQUIRE(weight == 3);

	// Shortest path from C to D: C->A->D (cost 3) or C->A->B->D (cost 4)
	std::vector<Node*> path_cd = dijkstra.findShortestPath(c, d);
	REQUIRE(path_cd.size() == 3);
	REQUIRE(path_cd[0]->label == "C");
	REQUIRE(path_cd[2]->label == "D");
	int weight_cd = graph.getCostOfPath(path_cd);
	REQUIRE(weight_cd == 3);

	// Clean up
	delete ab;
	delete bc;
	delete ca;
	delete ad;
	delete dc;
	delete bd;
	delete a;
	delete b;
	delete c;
	delete d;
}