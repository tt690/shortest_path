#include <vector>

#include "../../src/include/node.hpp"
#include "../../src/include/edge.hpp"
#include "../../src/include/graph.hpp"


struct MockGraph {
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    Graph* graph;
    Node *a, *b, *c, *d;
    Edge *ab, *bc, *cd;
    MockGraph() {
        a = new Node("A");
        b = new Node("B");
        c = new Node("C");
        d = new Node("D");
        ab = new Edge(a, b, 1);
        bc = new Edge(b, c, 1);
        cd = new Edge(c, d, 1);
        a->edges.push_back(ab);
        b->edges.push_back(bc);
        c->edges.push_back(cd);
        nodes = {a, b, c, d};
        edges = {ab, bc, cd};
        graph = new Graph(nodes, edges);
    }
    ~MockGraph() {
        delete ab; delete bc; delete cd;
        delete a; delete b; delete c; delete d;
        delete graph;
    }
};


struct ComplexMockGraph {
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    Graph* graph;
    Node *a, *b, *c, *d, *e, *f, *g;
    Edge *ab, *ac, *bc, *bd, *cd, *ce, *de, *ef, *fg, *ag, *be;
    ComplexMockGraph() {
        a = new Node("A");
        b = new Node("B");
        c = new Node("C");
        d = new Node("D");
        e = new Node("E");
        f = new Node("F");
        g = new Node("G");
        ab = new Edge(a, b, 2);
        ac = new Edge(a, c, 5);
        bc = new Edge(b, c, 2);
        bd = new Edge(b, d, 4);
        cd = new Edge(c, d, 1);
        ce = new Edge(c, e, 4);
        de = new Edge(d, e, 2);
        ef = new Edge(e, f, 1);
        fg = new Edge(f, g, 2);
        ag = new Edge(a, g, 20); // long direct edge
        be = new Edge(b, e, 10); // alternate longer path
        a->edges = {ab, ac, ag};
        b->edges = {bc, bd, be};
        c->edges = {cd, ce};
        d->edges = {de};
        e->edges = {ef};
        f->edges = {fg};
        nodes = {a, b, c, d, e, f, g};
        edges = {ab, ac, bc, bd, cd, ce, de, ef, fg, ag, be};
        graph = new Graph(nodes, edges);
    }
    ~ComplexMockGraph() {
        for (Edge* e : edges) delete e;
        for (Node* n : nodes) delete n;
        delete graph;
    }
};