#include "include/dijkstra.hpp"

Dijkstra::Dijkstra(Graph& graph) : graph(graph) {}

std::vector<Node*> Dijkstra::findShortestPath(Node* start, Node* end)
{
    // Lambda for priority queue: smallest distance first
    auto cmp = [](Node* a, Node* b) { return a->distanceToSource > b->distanceToSource; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> queue(cmp);

    // Initialize all nodes
    for (Node* node : graph.nodes) {
        node->distanceToSource = INT_MAX;
        node->previousNode = nullptr;
    }
    start->distanceToSource = 0;
    queue.push(start);

    while (!queue.empty()) {
        Node* current = queue.top();
        queue.pop();

        for (Edge* edge : current->edges) {
            Node* neighbor = edge->to;
            int newDist = current->distanceToSource + edge->cost;
            if (newDist < neighbor->distanceToSource) {
                neighbor->distanceToSource = newDist;
                neighbor->previousNode = current;
                queue.push(neighbor);
            }
        }
    }

    // Reconstruct path
    std::vector<Node*> route;
    Node* node = end;
    if (node->distanceToSource == INT_MAX) return route; // unreachable
    while (node) {
        route.insert(route.begin(), node);
        node = node->previousNode;
    }
    return route;
}