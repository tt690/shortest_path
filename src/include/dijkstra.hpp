#pragma once

#include <vector>

#include "graph.hpp"

class Dijkstra
{
    private:
        Graph& graph;
    public:
        Dijkstra(Graph& graph);
        void findShortestPath(Node* start, Node* end);
        std::vector<Node*> reconstructPath(Node* start, Node* end);
};