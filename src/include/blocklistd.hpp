#pragma once

#include <map>
#include <list>
#include <vector>

#include "node.hpp"

class BlockListD {
    public:
        struct Pair { Node* key; int value; };
        int M, B;
        std::list<std::vector<Pair>> D0, D1; // blocks
        std::map<Node*, Pair> keyMap; // for fast lookup

        BlockListD(int M_, int B_);
        void insert(Node* key, int value);
        void batchPrepend(const std::vector<Pair>& L);
        void remove(Node* key);
        bool empty() const;
        std::pair<std::vector<Node*>, int> pull();

    private:
        void splitBlock(std::vector<Pair>& block);
};
