#include <iostream>
#include <vector>
#include <string>

#include "src/include/node.hpp"
#include "src/include/edge.hpp"
#include "src/include/graph.hpp"

int main() {
    // Create nodes
    Node* a = new Node("A");
    Node* b = new Node("B");
    Node* c = new Node("C");

    // Create edges (A->B:1, B->C:2, A->C:4)
    Edge* ab = new Edge(a, b, 1);
    Edge* bc = new Edge(b, c, 2);
    Edge* ac = new Edge(a, c, 4);

    // Attach edges to nodes
    a->edges.push_back(ab);
    a->edges.push_back(ac);
    b->edges.push_back(bc);

    // Build graph
    std::vector<Node*> nodes = {a, b, c};
    std::vector<Edge*> edges = {ab, bc, ac};
    Graph graph(nodes, edges);

    // Find shortest path from A to C
    std::vector<Node*> path = graph.getShortestPath(c);

    // Print path
    std::cout << "Shortest path from A to C: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i]->label;
        if (i + 1 < path.size()) std::cout << " -> ";
    }
    std::cout << std::endl;

    // Print cost
    int cost = graph.getCostOfPath(path);
    std::cout << "Total cost: " << cost << std::endl;

    // Clean up
    delete ab;
    delete bc;
    delete ac;
    delete a;
    delete b;
    delete c;
    return 0;
}