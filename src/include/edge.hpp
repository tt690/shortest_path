#pragma once

#include "node.hpp"

class Edge
{
    public:
        Node* from;
        Node* to;
        int weight;

        Edge(Node* from, Node* to, int weight);

        friend std::ostream &operator<<(std::ostream& os, Edge& edge);
};