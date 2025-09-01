#pragma once

#include <random>

#include "graph.hpp"
#include "dijkstra.hpp"

class shortest_path {
    private:
        Graph& graph;
        Dijkstra* algorithm = nullptr;
        std::default_random_engine generator;

        Node *start_node = nullptr;
        Node *end_node = nullptr;

        int detect_graph_degree();

    public:
        shortest_path() : graph(*(new Graph({}, {}))) {}
        ~shortest_path() { delete algorithm; }

        void set_random_seed(unsigned seed);
        void generate_graph(int num_vertices, int num_edges);
        void set_start_end_nodes();

        std::pair<int, int> initialize(bool bmssp);
        void get_shortest_path();
};