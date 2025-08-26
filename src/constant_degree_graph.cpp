#include "include/constant_degree_graph.hpp"
#include <unordered_map>

ConstantDegreeGraph::ConstantDegreeGraph(const Graph& original, size_t /*degree*/)
    : Graph({}, {})
{
    std::unordered_map<Node*, std::vector<Node*>> cycleNodes;
    std::unordered_map<Edge*, Node*> edgeToCycleNodeFrom;
    std::unordered_map<Edge*, Node*> edgeToCycleNodeTo;

    int nextId = 0;
    // Step 1: For each node, create cycle nodes for each incident edge
    for (Node* v : original.nodes) {
        std::vector<Edge*> incidentEdges;
        // Collect all incoming and outgoing edges
        for (Edge* e : original.edges) {
            if (e->from == v || e->to == v) {
                incidentEdges.push_back(e);
            }
        }
        std::vector<Node*> cycle;
        for (size_t i = 0; i < incidentEdges.size(); ++i) {
            Node* xvw = new Node(v->label + std::to_string(nextId++)); // Preserve original label, unique id
            cycle.push_back(xvw);
            nodes.push_back(xvw);
        }
        cycleNodes[v] = cycle;
        // Map edges to their cycle nodes
        for (size_t i = 0; i < incidentEdges.size(); ++i) {
            Edge* e = incidentEdges[i];
            if (e->from == v) edgeToCycleNodeFrom[e] = cycle[i];
            if (e->to == v) edgeToCycleNodeTo[e] = cycle[i];
        }
        // Connect cycle nodes in a cycle with zero-weight edges
        size_t n = cycle.size();
        for (size_t i = 0; i < n; ++i) {
            Edge* zeroEdge = new Edge(cycle[i], cycle[(i+1)%n], 0);
            cycle[i]->edges.push_back(zeroEdge);
            edges.push_back(zeroEdge);
        }
    }

    // Step 2: For each edge (u, v) in original, add edge from xuv to xvu with original weight
    for (Edge* e : original.edges) {
        Node* xuv = edgeToCycleNodeFrom.count(e) ? edgeToCycleNodeFrom[e] : nullptr;
        Node* xvu = edgeToCycleNodeTo.count(e) ? edgeToCycleNodeTo[e] : nullptr;
        if (xuv && xvu) {
            Edge* newEdge = new Edge(xuv, xvu, e->weight);
            xuv->edges.push_back(newEdge);
            edges.push_back(newEdge);
        }
    }
}