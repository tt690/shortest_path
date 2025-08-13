#pragma once

#include <queue>
#include <algorithm>

#include "node.hpp"
#include "edge.hpp"

class Node;  // Forward declaration

class Graph
{
    public:
        std::vector<Node*> nodes;
        std::vector<Edge*> edges;
        Graph(std::vector<Node*> nodes, std::vector<Edge*> edges);
        Edge* getEdgeBetweenNodes(Node* from, Node* to);
        int getCostOfPath(std::vector<Node*> nodesToVisit);
};