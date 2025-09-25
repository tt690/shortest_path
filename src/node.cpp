#include "include/node.hpp"

#include <climits>

Node::Node(std::string label):label(label)
{
    this->distanceToSource = INT_MAX;
};

Node::Node(std::string label, int distanceToSource):label(label)
{
    this->distanceToSource = distanceToSource;
}

std::ostream &operator<<(std::ostream& os, Node& node) 
{
    os << "Node " << node.label << "\n";
    return os;
}

bool operator<(const Node& lhs, const Node& rhs)
{
    return lhs.distanceToSource < rhs.distanceToSource;
}