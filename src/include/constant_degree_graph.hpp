#pragma once
#include "graph.hpp"

class ConstantDegreeGraph : public Graph {
public:
    ConstantDegreeGraph(const Graph& original, size_t degree);
};
