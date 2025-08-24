#pragma once

#include <random>

#include "graph.hpp"
#include "dijkstra.hpp"

class shortest_path {
    private:
        Graph& graph;
        Dijkstra* algorithm = nullptr;
        std::default_random_engine generator;

        int detect_graph_degree();
        std::pair<Node*, Node*> get_start_end_nodes();

    public:
        shortest_path() : graph(*(new Graph({}, {}))) {}
        ~shortest_path() { delete &graph; }

        void generate_graph(int num_vertices, int num_edges);
        std::pair<int, int> initialize(bool bmssp);
        void get_shortest_path();
        void set_random_seed(unsigned seed);
};