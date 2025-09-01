#include "include/shortest_path.hpp"

#include <random>
#include <vector>
#include <unordered_set>

#include "include/graph.hpp"
#include "include/edge.hpp"
#include "include/node.hpp"
#include "include/dijkstra.hpp"
#include "include/bmssp.hpp"

struct InitResult {
    int vertex_count;
    int edge_count;
};

shortest_path shortest_path_instance;

extern "C" {
    __declspec(dllexport) void set_random_seed(unsigned seed) {
        shortest_path_instance.set_random_seed(seed);
    }

    __declspec(dllexport) void generate_graph(int num_vertices, int num_edges) {
        shortest_path_instance.generate_graph(num_vertices, num_edges);
    }

    __declspec(dllexport) void set_start_end_nodes() {
        shortest_path_instance.set_start_end_nodes();
    }

    __declspec(dllexport) InitResult initialize(bool bmssp) {
        auto result = shortest_path_instance.initialize(bmssp);
        InitResult out;
        out.vertex_count = result.first;
        out.edge_count = result.second;
        return out;
    }

    __declspec(dllexport) void get_shortest_path() {
        shortest_path_instance.get_shortest_path();
    }
}

void shortest_path::set_random_seed(unsigned seed) {
    std::srand(seed);
    this->generator.seed(seed);
}

// ...existing code...
void shortest_path::generate_graph(int num_vertices, int num_edges) {
    for (Node* n : graph.nodes) delete n;
    for (Edge* e : graph.edges) delete e;
    graph.nodes.clear();
    graph.edges.clear();

    std::uniform_int_distribution<int> weight_distribution(1, 10);

    std::vector<Node*> nodes;
    std::vector<Edge*> edges;

    nodes.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        Node* node = new Node(std::to_string(i));
        nodes.push_back(node);
    }

    // Parameters: adjust MAX_DEGREE to keep graph in BMSSP's constant-degree regime.
    const int MAX_DEGREE = 8; // choose 2..10 depending on target
    edges.reserve(num_edges);

    // keep track of out-degree and seen directed edges (u->v)
    std::vector<int> outdeg(num_vertices, 0);
    std::unordered_set<uint64_t> seen;
    seen.reserve(num_edges * 2);

    // 1) Build a simple spanning path/tree to ensure connectivity
    for (int i = 1; i < num_vertices && (int)edges.size() < num_edges; ++i) {
        int u = i - 1;
        int v = i;
        int weight = weight_distribution(this->generator);
        Edge* edge = new Edge(nodes[u], nodes[v], weight);
        edges.push_back(edge);
        outdeg[u]++;
        seen.insert((uint64_t(u) << 32) | uint32_t(v));
    }

    // 2) Add random edges while enforcing MAX_DEGREE and avoiding duplicates/self-loops.
    std::uniform_int_distribution<int> vertex_distribution(0, num_vertices - 1);
    int attempts = 0;
    const int max_attempts = std::max(10 * num_edges, 100000);
    while ((int)edges.size() < num_edges && attempts < max_attempts) {
        ++attempts;
        int u = vertex_distribution(this->generator);
        int v = vertex_distribution(this->generator);
        if (u == v) continue;
        if (outdeg[u] >= MAX_DEGREE) continue;
        uint64_t key = (uint64_t(u) << 32) | uint32_t(v);
        if (seen.find(key) != seen.end()) continue;
        int weight = weight_distribution(this->generator);
        Edge* edge = new Edge(nodes[u], nodes[v], weight);
        edges.push_back(edge);
        outdeg[u]++;
        seen.insert(key);
    }

    // If we failed to reach num_edges due to degree caps, num_edges will be smaller.
    this->graph.edges = std::move(edges);
    this->graph.nodes = std::move(nodes);
}

void shortest_path::set_start_end_nodes() {
    int start = rand() % graph.nodes.size();
    int end = rand() % graph.nodes.size();
    start_node = graph.nodes[start];
    end_node = graph.nodes[end];
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

std::pair<int, int> shortest_path::initialize(bool bmssp) {
    if (this->algorithm) delete this->algorithm;

    if (bmssp) {
        int degree = detect_graph_degree();
        this->algorithm = new BMSSP(this->graph, degree);
    } else {
        this->algorithm = new Dijkstra(this->graph);
    }

    int vertex_count = this->algorithm->graph.nodes.size();
    int edges_count = this->algorithm->graph.edges.size();

    return {vertex_count, edges_count};
}

void shortest_path::get_shortest_path() {
    if (this->algorithm) {
        this->algorithm->find_shortest_path(this->start_node, this->end_node);
    }
}