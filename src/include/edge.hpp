#pragma once

#include "node.hpp"

class Edge
{
    public:
        Node* from;
        Node* to;
        int cost;

        Edge(Node* from, Node* to, int cost);

        friend std::ostream &operator<<(std::ostream& os, Edge& edge);
};