#include "include/bmssp.hpp"

#include <map>
#include <list>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

#include "include/node.hpp"
#include "include/edge.hpp"

#include "include/blocklistd.hpp"
#include "include/constant_degree_graph.hpp"


BMSSP::BMSSP(Graph& inputGraph, size_t degree) 
    : graph(inputGraph)
{
    bool alreadyConstant = true;
    for (Node* node : inputGraph.nodes) {
        if (node->edges.size() != degree) { alreadyConstant = false; break; }
    }
    if (!alreadyConstant) {
        ConstantDegreeGraph cdGraph(inputGraph, degree);
        this->graph = cdGraph;
    }

    const int n = static_cast<int>(this->graph.nodes.size());
    // Guard for tiny graphs
    const double ln_n = std::max(1e-9, std::log(std::max(2, n)));  // avoid log(1)~0/underflow

    // k := floor( (log n)^(1/3) ),  t := floor( (log n)^(2/3) )
    this->k = std::max(1, static_cast<int>(std::floor(std::pow(ln_n, 1.0/3.0))));
    this->t = std::max(1, static_cast<int>(std::floor(std::pow(ln_n, 2.0/3.0))));

    // l in [0, ceil(log n / t)]
    this->l = std::max(1, static_cast<int>(std::ceil(ln_n / static_cast<double>(this->t))));
}

std::pair<std::vector<Node*>, std::vector<Node*>> BMSSP::findNodes(int upperbound, std::vector<Node*> vertices)
{
    std::vector<Node*> weight = vertices; // W
    std::vector<std::vector<Node*>> weights; // Wi
    weights.push_back(vertices); // W0

    for (int i = 1; i <= this->k; i++)
    {
        weights.push_back(std::vector<Node*>()); // Wi

        // Efficient lookup for previous set
        std::unordered_set<Node*> prevSet(weights[i - 1].begin(), weights[i - 1].end());

        for (Edge* edge : this->graph.edges) {
            Node* u = edge->from;
            Node* v = edge->to;
            if (prevSet.count(u)) {
                if (u->distanceToSource + edge->weight <= v->distanceToSource) {
                    v->distanceToSource = u->distanceToSource + edge->weight;
                    v->previousNode = u;
                    if (u->distanceToSource + edge->weight < upperbound) {
                        weights[i].push_back(v);
                    }
                }
            }
        }

        // Add Wi to W
        weight.insert(weight.end(), weights[i].begin(), weights[i].end());

        if (weight.size() > this->k * vertices.size()) {
            return {vertices, weight}; // vertices as P
        }
    }

    // Build directed forest
    std::vector<Edge*> directedForest;
    std::unordered_set<Node*> weightSet(weight.begin(), weight.end());
    for (Edge* edge : this->graph.edges) {
        if (weightSet.count(edge->from) && weightSet.count(edge->to)) {
            Node* fromNode = edge->from;
            Node* toNode = edge->to;
            if (toNode->distanceToSource == fromNode->distanceToSource + edge->weight) {
                directedForest.push_back(edge);
            }
        }
    }

    // Find pivots: roots in vertices with >= k descendants in directedForest
    std::vector<Node*> pivots;
    for (Node* u : vertices) {
        std::unordered_set<Node*> visited;
        std::queue<Node*> q;
        q.push(u);
        visited.insert(u);
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            for (Edge* edge : directedForest) {
                if (edge->from == curr && !visited.count(edge->to)) {
                    visited.insert(edge->to);
                    q.push(edge->to);
                }
            }
        }
        if (visited.size() >= this->k) {
            pivots.push_back(u);
        }
    }
    return {pivots, weight};
}

// PATCH: baseCase that matches Algorithm 2 more closely
std::pair<int, std::vector<Node*>> BMSSP::baseCase(int upperbound, std::vector<Node*> vertices)
{
    // Assumption: vertices == {x} and x is complete
    Node* x = vertices.front();

    std::vector<Node*> U0;           // U0 ← S initially
    U0.push_back(x);
    std::unordered_set<Node*> inU0{ x };

    auto cmp = [](Node* a, Node* b) { return a->distanceToSource > b->distanceToSource; };
    std::vector<Node*> heap{ x };    // heap initialized with the singleton {x}
    std::make_heap(heap.begin(), heap.end(), cmp);
    std::unordered_set<Node*> inHeap{ x };

    // Extract until we gather k+1 vertices or heap empty
    while (!heap.empty() && static_cast<int>(U0.size()) < k + 1) {
        std::pop_heap(heap.begin(), heap.end(), cmp);
        Node* u = heap.back(); heap.pop_back();
        inHeap.erase(u);

        if (!inU0.count(u)) {
            U0.push_back(u);
            inU0.insert(u);
        }

        // Relax outgoing edges of u only (respecting db[u] + w <= db[v] and bound B)
        for (Edge* e : u->edges) {
            Node* v = e->to;
            const int cand = (u->distanceToSource == INT_MAX) ? INT_MAX : u->distanceToSource + e->weight;
            if (cand <= v->distanceToSource && cand < upperbound) {  // NOTE: ≤ as in Remark 3.4
                if (cand < v->distanceToSource) {
                    v->distanceToSource = cand;
                    v->previousNode = u;
                }
                if (!inHeap.count(v)) {
                    heap.push_back(v);
                    std::push_heap(heap.begin(), heap.end(), cmp);
                    inHeap.insert(v);
                } else {
                    // cheap decrease-key: rebuild heap (fine here due to small k)
                    std::make_heap(heap.begin(), heap.end(), cmp);
                }
            }
        }
    }

    if (static_cast<int>(U0.size()) <= k) {
        // Successful execution: B' = B, U = U0
        return { upperbound, U0 };
    } else {
        // Found k+1 vertices: set B' = max_{v in U0} d(v), and U = { v in U0 : d(v) < B' }
        int Bprime = 0;
        for (Node* v : U0) {
            if (v->distanceToSource != INT_MAX)
                Bprime = std::max(Bprime, v->distanceToSource);
        }
        std::vector<Node*> U;
        for (Node* v : U0) {
            if (v->distanceToSource < Bprime) U.push_back(v);
        }
        return { Bprime, U };
    }
}

// Helper (optional): capacity/threshold at level l
inline size_t cap_at_level(int l, int k, int t) {
    // k^2 * 2^{l*t}
    const double pow2 = std::pow(2.0, static_cast<double>(l) * static_cast<double>(t));
    const double capd = static_cast<double>(k) * static_cast<double>(k) * pow2;
    return static_cast<size_t>(std::ceil(capd));
}

std::pair<int, std::vector<Node*>> BMSSP::bmssp(int l, int B, std::vector<Node*> S)
{
    if (l == 0) {
        return baseCase(B, S);
    }

    auto pivotsResult = findNodes(B, S);
    std::vector<Node*> P = pivotsResult.first;
    std::vector<Node*> W = pivotsResult.second;

    // PATCH: M := 2^{(l-1)*t}
    const size_t M = (l <= 0) ? 1ull : static_cast<size_t>(std::max(1.0, std::pow(2.0, static_cast<double>(l - 1) * static_cast<double>(t))));
    BlockListD D(static_cast<int>(M), B);

    for (Node* x : P) {
        D.insert(x, x->distanceToSource);
    }

    // Track the minimum B'_i seen so we can return min{B'_i, B}
    int minBprimeAcrossPhases = B;               // initialize to B (neutral for min)
    std::vector<Node*> U;                        // accumulated U
    const size_t cap = cap_at_level(l, k, t);    // k^2 * 2^{l*t}

    while (U.size() < cap && !D.empty()) {
        // ---- Phase i ----
        auto [Si_nodes, Bi] = D.pull();          // Pull() returns up to M keys and the separating bound Bi

        // Recursive call
        auto rec = bmssp(l - 1, Bi, Si_nodes);
        const int Bprime_i = rec.first;
        std::vector<Node*> Ui = std::move(rec.second);

        // Accumulate
        if (!Ui.empty()) {
            U.insert(U.end(), Ui.begin(), Ui.end());
        }
        minBprimeAcrossPhases = std::min(minBprimeAcrossPhases, Bprime_i);

        // Relax edges from u ∈ Ui
        std::vector<BlockListD::Pair> K; K.reserve(Ui.size());
        for (Node* u : Ui) {
            for (Edge* e : u->edges) {
                Node* v = e->to;
                const int cand = (u->distanceToSource == INT_MAX) ? INT_MAX : u->distanceToSource + e->weight;
                if (cand <= v->distanceToSource) {                 // NOTE: ≤ as in Remark 3.4
                    if (cand < v->distanceToSource) {
                        v->distanceToSource = cand;
                        v->previousNode = u;
                    }
                    if (cand >= Bi && cand < B) {
                        D.insert(v, cand);
                    } else if (cand >= Bprime_i && cand < Bi) {
                        K.push_back({ v, cand });
                    }
                }
            }
        }

        // BatchPrepend K and {<x, db[x]> : x ∈ Si, db[x] ∈ [B'_i, Bi)}
        if (!K.empty()) D.batchPrepend(K);
        for (Node* x : Si_nodes) {
            const int dx = x->distanceToSource;
            if (dx >= Bprime_i && dx < Bi) {
                D.batchPrepend({ BlockListD::Pair{ x, dx } });
            }
        }

        // Early exits per Algorithm 3
        if (D.empty()) {
            // Successful execution at this node
            const int Bret = std::min(minBprimeAcrossPhases, B);
            // U ← U ∪ { x ∈ W : db[x] < B' }
            for (Node* x : W) if (x->distanceToSource < Bret) U.push_back(x);
            return { Bret, U };
        }
        if (U.size() > cap) {
            // Partial execution due to large workload
            const int Bret = Bprime_i;           // set to the current phase’s B'_i
            for (Node* x : W) if (x->distanceToSource < Bret) U.push_back(x);
            return { Bret, U };
        }
    }

    // End of subroutine: return B' ← min{min_i B'_i, B}, and add W' to U
    const int Bfinal = std::min(minBprimeAcrossPhases, B);
    for (Node* x : W) if (x->distanceToSource < Bfinal) U.push_back(x);
    return { Bfinal, U };
}


std::vector<Node*> BMSSP::findShortestPath(Node* start, Node* end)
{
    // Set source node distance to 0 and clear previousNode for all nodes
    for (Node* node : this->graph.nodes) {
        node->distanceToSource = INT_MAX;
        node->previousNode = nullptr;
    }
    start->distanceToSource = 0;

    int B = INT_MAX;
    std::vector<Node*> S = {start};
    auto result = bmssp(this->l, B, S);
    // Reconstruct path from start to end using previousNode
    std::vector<Node*> path;
    Node* node = end;
    if (node->distanceToSource == INT_MAX) return path; // unreachable
    while (node) {
        path.insert(path.begin(), node);
        node = node->previousNode;
    }
    return path;
}