#include "include/catch.hpp"

#include "include/mock_graphs.hpp"
#include "../src/include/constant_degree_graph.hpp"

TEST_CASE("Transforming to constant degree graph sets all node degrees correctly", "[constant_degree_graph]") {
	MockGraph mg;
	size_t targetDegree = 2;
	ConstantDegreeGraph cdGraph(*mg.graph, targetDegree);

	for (Node* node : cdGraph.nodes) {
		REQUIRE(node->edges.size() <= targetDegree);
	}
}

TEST_CASE("ConstantDegreeGraph maintains constant degree, even with complex graphs", "[constant_degree_graph]") {
	ComplexMockGraph mg;
	size_t targetDegree = 3;
	ConstantDegreeGraph cdGraph(*mg.graph, targetDegree);

	for (Node* node : cdGraph.nodes) {
		REQUIRE(node->edges.size() <= targetDegree);
	}
}