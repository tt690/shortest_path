#include "include/shortest_path.hpp"

#include <random>
#include <vector>

#include "include/graph.hpp"
#include "include/edge.hpp"
#include "include/node.hpp"
#include "include/dijkstra.hpp"
#include "include/bmssp.hpp"

shortest_path shortest_path_instance;

extern "C" {
    __declspec(dllexport) void generate_graph(int num_vertices, int num_edges) {
        shortest_path_instance.generate_graph(num_vertices, num_edges);
    }

    __declspec(dllexport) void initialize(bool bmssp) {
        shortest_path_instance.initialize(bmssp);
    }

    __declspec(dllexport) void get_shortest_path() {
        shortest_path_instance.get_shortest_path();
    }
}

void shortest_path::generate_graph(int num_vertices, int num_edges) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> vertex_distribution(0, num_vertices - 1);
    std::uniform_int_distribution<int> weight_distribution(1, 10);

    std::vector<Node*> nodes;
    std::vector<Edge*> edges;

    for (int i = 0; i < num_vertices; ++i) {
        Node* node = new Node(std::to_string(i));
        nodes.push_back(node);
    }

    for (int i = 0; i < num_edges; ++i) {
        int u = vertex_distribution(generator);
        int v = vertex_distribution(generator);
        int weight = weight_distribution(generator);
        Edge* edge = new Edge(nodes[u], nodes[v], weight);
        edges.push_back(edge);
    }

    this->graph = Graph(nodes, edges);
}

int shortest_path::detect_graph_degree() {
    int max_degree = 0;
    for (const Node* node : this->graph.nodes) {
        if (static_cast<int>(node->edges.size()) > max_degree) {
            max_degree = static_cast<int>(node->edges.size());
        }
    }
    return max_degree;
}

void shortest_path::initialize(bool bmssp) {
    if (bmssp) {
        int degree = detect_graph_degree();
        this->algorithm = new BMSSP(this->graph, degree);
    } else {
        this->algorithm = new Dijkstra(this->graph);
    }
}

std::pair<Node*, Node*> shortest_path::get_start_end_nodes() {
    int start = rand() % graph.nodes.size();
    int end = rand() % graph.nodes.size();
    return {graph.nodes[start], graph.nodes[end]};
}

void shortest_path::get_shortest_path() {
    auto [start, end] = get_start_end_nodes();

    if (this->algorithm) {
        this->algorithm->find_shortest_path(start, end);
    }
}