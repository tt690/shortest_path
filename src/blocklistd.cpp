#include "include/blocklistd.hpp"
#include <algorithm>

BlockListD::BlockListD(int M_, int B_) : M(M_), B(B_) {
    D1.push_back({});
}

void BlockListD::insert(Node* key, int value) {
    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        if (value < it->second.value) {
            remove(key);
        } else {
            return;
        }
    }
    D1.front().push_back({key, value});
    keyMap[key] = {key, value};
    if (D1.front().size() > M) splitBlock(D1.front());
}

void BlockListD::batchPrepend(const std::vector<Pair>& L) {
    if (L.empty()) return;
    std::vector<Pair> block;
    for (const auto& p : L) {
        auto it = keyMap.find(p.key);
        if (it == keyMap.end() || p.value < it->second.value) {
            keyMap[p.key] = p;
            block.push_back(p);
            if (block.size() == M) {
                D0.push_front(block);
                block.clear();
            }
        }
    }
    if (!block.empty()) D0.push_front(block);
}

void BlockListD::remove(Node* key) {
    keyMap.erase(key);
    for (auto& block : D0) block.erase(std::remove_if(block.begin(), block.end(), [key](const Pair& p){return p.key==key;}), block.end());
    for (auto& block : D1) block.erase(std::remove_if(block.begin(), block.end(), [key](const Pair& p){return p.key==key;}), block.end());
}

bool BlockListD::empty() const { return keyMap.empty(); }

std::pair<std::vector<Node*>, int> BlockListD::pull() {
    std::vector<Pair> all;
    for (auto& block : D0) all.insert(all.end(), block.begin(), block.end());
    for (auto& block : D1) all.insert(all.end(), block.begin(), block.end());
    std::sort(all.begin(), all.end(), [](const Pair& a, const Pair& b){return a.value < b.value;});
    std::vector<Node*> S;
    int x = B;
    for (size_t i = 0; i < all.size() && S.size() < M; ++i) {
        S.push_back(all[i].key);
    }
    if (S.size() < all.size()) x = all[S.size()].value;
    for (Node* k : S) remove(k);
    return {S, x};
}

void BlockListD::splitBlock(std::vector<Pair>& block) {
    std::sort(block.begin(), block.end(), [](const Pair& a, const Pair& b){return a.value < b.value;});
    std::vector<Pair> newBlock(block.begin() + M/2, block.end());
    block.erase(block.begin() + M/2, block.end());
    D1.insert(++D1.begin(), newBlock);
}