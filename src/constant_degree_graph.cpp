#include "include/constant_degree_graph.hpp"
#include <random>

ConstantDegreeGraph::ConstantDegreeGraph(const Graph& original, size_t degree)
    : Graph(original.nodes, original.edges) // Start with original nodes/edges
{
    // For each node, adjust its degree to match 'degree'
    std::random_device rd;
    std::mt19937 gen(rd());
    for (Node* node : nodes) {
        // Remove excess edges
        while (node->edges.size() > degree) {
            node->edges.pop_back();
        }
        // Add random edges if degree is too low
        while (node->edges.size() < degree) {
            // Pick a random target node (not self, not already connected)
            std::vector<Node*> candidates;
            for (Node* target : nodes) {
                if (target != node) {
                    bool alreadyConnected = false;
                    for (Edge* e : node->edges) {
                        if (e->to == target) {
                            alreadyConnected = true;
                            break;
                        }
                    }
                    if (!alreadyConnected) candidates.push_back(target);
                }
            }
            if (candidates.empty()) break;
            std::uniform_int_distribution<> dis(0, candidates.size() - 1);
            Node* target = candidates[dis(gen)];
            
            // Add edge (cost=0)
            Edge* newEdge = new Edge(node, target, 0);
            node->edges.push_back(newEdge);
            edges.push_back(newEdge);
        }
    }
}
