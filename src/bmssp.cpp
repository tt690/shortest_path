#include "include/bmssp.hpp"

#include <map>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

#include "include/node.hpp"
#include "include/edge.hpp"
#include "include/blocklistd.hpp"
#include "include/constant_degree_graph.hpp"


BMSSP::BMSSP(Graph& inputGraph, size_t degree) 
    : Dijkstra(inputGraph), graph(inputGraph)
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
    
    const double ln_n = std::max(1e-9, std::log(std::max(2, n)));  // avoid log(1)~0/underflow

    // k := floor( (log n)^(1/3) ),  t := floor( (log n)^(2/3) )
    this->k = std::max(1, static_cast<int>(std::floor(std::pow(ln_n, 1.0/3.0))));
    this->t = std::max(1, static_cast<int>(std::floor(std::pow(ln_n, 2.0/3.0))));

    // l in [0, ceil(log n / t)]
    this->l = std::max(1, static_cast<int>(std::ceil(ln_n / static_cast<double>(this->t))));
}

std::pair<std::vector<Node*>, std::vector<Node*>> BMSSP::findNodes(int upperbound, std::vector<Node*> vertices)
{
    std::vector<Node*> W = vertices; // W ← S
    std::vector<std::vector<Node*>> Wi; // Wi for k rounds
    Wi.push_back(vertices); // W0 ← S

    // Reusable container to mark membership of previous round
    std::unordered_set<Node*> prevSet;
    prevSet.reserve(std::max<size_t>(vertices.size()*2, 16));

    for (int i = 1; i <= this->k; ++i) {
        Wi.push_back({});
        prevSet.clear();
        prevSet.reserve(std::max<size_t>(Wi[i-1].size()*2, 16));
        for (Node* n : Wi[i-1]) prevSet.insert(n);

        // Single full edge scan, but using the prebuilt set for membership testing
        for (Edge* edge : this->graph.edges) {
            Node* u = edge->from;
            Node* v = edge->to;
            if (prevSet.count(u)) {
                if (u->distanceToSource + edge->weight <= v->distanceToSource) {
                    v->distanceToSource = u->distanceToSource + edge->weight;
                    v->previousNode = u;
                    if (u->distanceToSource + edge->weight < upperbound) {
                        Wi[i].push_back(v);
                    }
                }
            }
        }
        W.insert(W.end(), Wi[i].begin(), Wi[i].end());
        if (W.size() > this->k * vertices.size()) {
            // Partial execution: return all of S as pivots
            return {vertices, W};
        }
    }

    // Build directed forest adjacency once: children[u] = list of v
    std::unordered_map<Node*, std::vector<Node*>> children;
    children.reserve(std::max<size_t>(W.size()*2, 16));
    std::unordered_set<Node*> Wset;
    Wset.reserve(std::max<size_t>(W.size()*2, 16));
    for (Node* n : W) Wset.insert(n);

    for (Edge* edge : this->graph.edges) {
        if (Wset.count(edge->from) && Wset.count(edge->to)) {
            if (edge->to->distanceToSource == edge->from->distanceToSource + edge->weight) {
                children[edge->from].push_back(edge->to);
            }
        }
    }

    // Find pivots: roots in S with >= k descendants in F
    std::vector<Node*> pivots;
    pivots.reserve(vertices.size());
    for (Node* u : vertices) {
        // BFS over children adjacency to count descendants
        std::unordered_set<Node*> visited;
        visited.reserve(std::max<size_t>(this->k * 2, 16));
        std::queue<Node*> q;
        q.push(u);
        visited.insert(u);
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            auto it = children.find(curr);
            if (it == children.end()) continue;
            for (Node* child : it->second) {
                if (!visited.count(child)) {
                    visited.insert(child);
                    q.push(child);
                }
            }
            // small early exit if we already reached k (saves work)
            if (static_cast<int>(visited.size()) >= this->k) break;
        }
        if (static_cast<int>(visited.size()) >= this->k) {
            pivots.push_back(u);
        }
    }
    return {pivots, W};
}

std::pair<int, std::vector<Node*>> BMSSP::baseCase(int upperbound, std::vector<Node*> vertices)
{
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

void BMSSP::find_shortest_path(Node* start, Node* end)
{
    // Set source node distance to 0 and other nodes to INT_MAX
    for (Node* node : this->graph.nodes) {
        node->distanceToSource = INT_MAX;
        node->previousNode = nullptr;
    }
    start->distanceToSource = 0;

    int B = INT_MAX;
    std::vector<Node*> S = {start};
    auto result = bmssp(this->l, B, S);
}