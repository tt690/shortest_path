#pragma once

#include "graph.hpp"
#include "dijkstra.hpp"

class shortest_path {
    private:
        Graph& graph;
        Dijkstra* algorithm = nullptr;
        int detect_graph_degree();
        std::pair<Node*, Node*> get_start_end_nodes();

    public:
        shortest_path() : graph(*(new Graph({}, {}))) {}
        ~shortest_path() { delete &graph; }

        void generate_graph(int num_vertices, int num_edges);
        void initialize(bool bmssp);
        void get_shortest_path();
};