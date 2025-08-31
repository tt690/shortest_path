#pragma once

#include <vector>

#include "graph.hpp"

class Dijkstra
{
    public:
        Graph& graph;
        Dijkstra(Graph& graph);
        virtual ~Dijkstra() {};
        virtual void find_shortest_path(Node* start, Node* end);
        std::vector<Node*> reconstruct_path(Node* start, Node* end);
};