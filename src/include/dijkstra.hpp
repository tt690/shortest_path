#pragma once

#include <vector>

#include "graph.hpp"

class Dijkstra
{
    private:
        Graph& graph;
    public:
        Dijkstra(Graph& graph);
        std::vector<Node*> findShortestPath(Node* start, Node* end);
};