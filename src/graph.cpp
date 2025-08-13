#include "include/graph.hpp"

Graph::Graph(std::vector<Node*> nodes, std::vector<Edge*> edges) : nodes(nodes),edges(edges){};

Edge* Graph::getEdgeBetweenNodes(Node* from, Node* to)
{
    for (Edge* edge : from->edges)
    {
        if (edge->to == to)
        {
            return edge;
        }
    }
    return nullptr;
}

int Graph::getCostOfPath(std::vector<Node*> nodesToVisit) 
{
    int totalCosts = 0;
    for (size_t i = 0; i + 1 < nodesToVisit.size(); i++)
    {
        totalCosts  += getEdgeBetweenNodes(nodesToVisit[i], nodesToVisit[i+1])->cost ;
    }
    //Calculate total costs 
    return totalCosts;
}

std::vector<Node*> Graph::getShortestPath(Node* target)
{
    // Lambda for priority queue: smallest distance first
    auto cmp = [](Node* a, Node* b) { return a->distanceToSource > b->distanceToSource; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> queue(cmp);

    // Initialize all nodes
    for (Node* node : this->nodes) {
        node->distanceToSource = INT_MAX;
        node->previousNode = nullptr;
    }
    // Assume first node is source (customize as needed)
    Node* source = this->nodes.empty() ? nullptr : this->nodes[0];
    if (!source) return {};
    source->distanceToSource = 0;
    queue.push(source);

    while (!queue.empty()) {
        Node* current = queue.top();
        queue.pop();

        // For each neighbor
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
    Node* node = target;
    if (node->distanceToSource == INT_MAX) return route; // unreachable
    while (node) {
        route.insert(route.begin(), node);
        node = node->previousNode;
    }
    return route;
}