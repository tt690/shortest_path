#pragma once

#include <vector>

#include "graph.hpp"

class Dijkstra
{
    private:
        Graph& graph;
        
    public:
        Dijkstra(Graph& graph);
        void find_shortest_path(Node* start, Node* end);
        std::vector<Node*> reconstruct_path(Node* start, Node* end);
};