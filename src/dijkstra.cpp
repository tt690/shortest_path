#include "include/dijkstra.hpp"

#include <queue>
#include <algorithm>

Dijkstra::Dijkstra(Graph& graph) : graph(graph) {}

void Dijkstra::find_shortest_path(Node* start, Node* end)
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
            int newDist = current->distanceToSource + edge->weight;
            if (newDist < neighbor->distanceToSource) {
                neighbor->distanceToSource = newDist;
                neighbor->previousNode = current;
                queue.push(neighbor);
            }
        }
    }
}

std::vector<Node*> Dijkstra::reconstruct_path(Node* start, Node* end)
{
    std::vector<Node*> path;
    Node* curr = end;
    while (curr != nullptr) {
        path.push_back(curr);
        if (curr == start) break;
        curr = curr->previousNode;
    }
    std::reverse(path.begin(), path.end());
    return path;
}