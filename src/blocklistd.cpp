#include "include/blocklistd.hpp"

#include <algorithm>
#include <set>
#include <map>
#include <iostream>


static size_t blockSize(int M) {
    return (M + 1) / 2; // ceil(M/2)
}


BlockListD::BlockListD(int M_, int B_) : M(M_), B(B_) {
    // D0: empty sequence (no blocks)
    D0.clear();
    // D1: single empty block with upper bound B
    D1.push_back({});
    D1Bounds[B] = D1.begin(); // map upper bound to block iterator
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

    // Find block with smallest upper bound >= value using map
    auto boundIt = D1Bounds.lower_bound(value);
    if (boundIt == D1Bounds.end()) --boundIt;
    auto blockIt = boundIt->second;

    // Insert sorted into block
    auto& block = *blockIt;
    auto pos = std::lower_bound(block.begin(), block.end(), value, [](const Pair& p, int v){return p.value < v;});
    block.insert(pos, {key, value});
    keyMap[key] = {key, value};

    // Update upper bound
    D1Bounds.erase(boundIt);
    D1Bounds[block.empty() ? B : block.back().value] = blockIt;
    while (block.size() > M) splitBlock(blockIt);
}


void BlockListD::batchPrepend(const std::vector<Pair>& L) {
    if (L.empty()) return;
    std::vector<Pair> batch = L;
    // Only keep unique keys with smallest value
    std::map<Node*, Pair> unique;
    for (const auto& p : batch) {
        auto it = unique.find(p.key);
        if (it == unique.end() || p.value < it->second.value) unique[p.key] = p;
    }
    std::vector<Pair> uniqueBatch;
    for (const auto& kv : unique) {
        keyMap[kv.first] = kv.second;
        uniqueBatch.push_back(kv.second);
    }
    size_t sz = blockSize(M);
    size_t idx = 0;
    while (idx < uniqueBatch.size()) {
        size_t take = std::min(sz, uniqueBatch.size() - idx);
        std::vector<Pair> block(uniqueBatch.begin() + idx, uniqueBatch.begin() + idx + take);
        D0.push_front(block);
        idx += take;
    }
}


void BlockListD::remove(Node* key) {
    keyMap.erase(key);

    for (auto& block : D0) {
        block.erase(std::remove_if(block.begin(), block.end(), [key](const Pair& p){return p.key==key;}), block.end());
    }

    for (auto it = D1.begin(); it != D1.end(); ++it) {
        size_t before = it->size();
        it->erase(std::remove_if(it->begin(), it->end(), [key](const Pair& p){return p.key==key;}), it->end());
        if (before > 0 && it->empty()) {
            std::vector<int> boundsToRemove;
            for (const auto& kv : D1Bounds) {
                if (kv.second == it) boundsToRemove.push_back(kv.first);
            }
            for (int bound : boundsToRemove) D1Bounds.erase(bound);
        }
    }

    // If all blocks in D1 are empty, reinitialize D1 and D1Bounds as in the constructor
    bool allEmpty = true;
    for (const auto& block : D1) {
        if (!block.empty()) {
            allEmpty = false;
            break;
        }
    }
    if (allEmpty) {
        D1.clear();
        D1.push_back({});
        D1Bounds.clear();
        D1Bounds[B] = D1.begin();
    }
}


bool BlockListD::empty() const { return keyMap.empty(); }


std::pair<std::vector<Node*>, int> BlockListD::pull() {
    // Always return a prefix of size ceil(M/2) (or all remaining if less)
    std::vector<Pair> candidates;
    for (auto& block : D0) {
        for (const auto& p : block) candidates.push_back(p);
        if (candidates.size() >= blockSize(M)) break;
    }
    for (auto& block : D1) {
        for (const auto& p : block) candidates.push_back(p);
        if (candidates.size() >= blockSize(M)) break;
    }

    // Find ceil(M/2) smallest
    size_t sz = blockSize(M);
    size_t take = std::min(sz, candidates.size());
    std::nth_element(candidates.begin(), candidates.begin() + take, candidates.end(), [](const Pair& a, const Pair& b){return a.value < b.value;});

    std::vector<Node*> S;
    int x = B;
    for (size_t i = 0; i < take; ++i) {
        S.push_back(candidates[i].key);
    }
    if (candidates.size() > take) {
        int nextVal = candidates[take].value;
        x = nextVal;
    }
    for (Node* k : S) remove(k);
    
    return {S, x};
}


void BlockListD::splitBlock(std::list<std::vector<Pair>>::iterator blockIt) {
    auto& block = *blockIt;
    size_t sz = blockSize(M);

    // Sort block by value
    std::sort(block.begin(), block.end(), [](const Pair& a, const Pair& b){return a.value < b.value;});
    std::vector<Pair> left(block.begin(), block.begin() + sz);
    std::vector<Pair> right(block.begin() + sz, block.end());
    block = left;
    auto newBlockIt = D1.insert(++blockIt, right);
    
    // Update bounds
    std::vector<int> boundsToRemove;
    for (const auto& kv : D1Bounds) {
        if (kv.second == blockIt) boundsToRemove.push_back(kv.first);
    }
    for (int bound : boundsToRemove) D1Bounds.erase(bound);
    int leftBound = left.empty() ? B : left.back().value;
    int rightBound = right.empty() ? B : right.back().value;
    D1Bounds[leftBound] = blockIt;
    D1Bounds[rightBound] = newBlockIt;
}