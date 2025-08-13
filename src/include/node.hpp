#pragma once

#include <string>
#include <iostream>
#include <vector>

class Edge;

class Node
{
    public:
        std::string label;
        std::vector<Edge*> edges;
        int distanceToSource;
        Node* previousNode;

        Node(std::string label);
        Node(std::string label, int distanceToSource);

        friend std::ostream &operator<<(std::ostream &os, Node& node);
        friend bool operator<(const Node& lhs, const Node& rhs);
};

