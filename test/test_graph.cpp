#include "include/catch.hpp"

#include "../src/include/constant_degree_graph.hpp"

TEST_CASE("Transforming to constant degree graph sets all node degrees correctly", "[constant_degree_graph]") {
	Node* a = new Node("A");
	Node* b = new Node("B");
	Node* c = new Node("C");
	Node* d = new Node("D");

	// Create edges (A->B, B->C, C->D)
	Edge* ab = new Edge(a, b, 1);
	Edge* bc = new Edge(b, c, 1);
	Edge* cd = new Edge(c, d, 1);
	a->edges.push_back(ab);
	b->edges.push_back(bc);
	c->edges.push_back(cd);

	std::vector<Node*> nodes = {a, b, c, d};
	std::vector<Edge*> edges = {ab, bc, cd};
	Graph graph(nodes, edges);

	size_t targetDegree = 2;
	ConstantDegreeGraph cdGraph(graph, targetDegree);

	for (Node* node : cdGraph.nodes) {
		REQUIRE(node->edges.size() <= targetDegree); // Out-degree at most targetDegree
	}

	// Clean up dynamically allocated edges and nodes
	for (Edge* edge : cdGraph.edges) delete edge;
	for (Node* node : cdGraph.nodes) delete node;
}