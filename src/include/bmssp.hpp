#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>

#include "graph.hpp"

class BMSSP
{
    private:
        Graph& graph;
        std::vector<Node*> pivots;
        int k, t, l;

    public:
        BMSSP(Graph& inputGraph, size_t degree);
        std::pair<std::vector<Node*>, std::vector<Node*>> findNodes(int upperbound, std::vector<Node*> vertices);  // Set pivots and weights, might be redundant as in already implemented in graph
        std::pair<int, std::vector<Node*>> baseCase(int upperbound, std::vector<Node*> vertices);  // Sets upperbound and S, vertices
        std::pair<int, std::vector<Node*>> bmssp(int length, int upperbound, std::vector<Node*> vertices);
        std::vector<Node*> findShortestPath(Node* start, Node* end);
};