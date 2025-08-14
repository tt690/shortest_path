#include "include/edge.hpp"

Edge::Edge(Node* from, Node* to, int weight): from(from), to(to), weight(weight){};

std::ostream &operator<<(std::ostream &os, Edge& edge) 
{
    os <<  "( " << edge.from->label << "->" << edge.to->label << ")" << "\n";
    return os;
}