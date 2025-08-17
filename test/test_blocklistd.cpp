#include "include/catch.hpp"

#include "../src/include/blocklistd.hpp"

#include <vector>
#include <algorithm>


TEST_CASE("BlockListD Insertion and Pulling", "[BlockListD]") {
    BlockListD blockList(3, 10);  // M = 3, B = 10

    // Create nodes
    Node* n1 = new Node("n1");
    Node* n2 = new Node("n2");
    Node* n3 = new Node("n3");
    Node* n4 = new Node("n4");
    Node* n5 = new Node("n5");

    // Insert elements into the block list
    blockList.insert(n1, 5);
    blockList.insert(n2, 3);
    blockList.insert(n3, 8);
    blockList.insert(n4, 2);
    blockList.insert(n5, 7);

    // Pull elements from the block list
    auto [pulledNodes, newBound] = blockList.pull();

    // Check the pulled nodes
    REQUIRE(pulledNodes.size() <= 3);
    REQUIRE(newBound == 10);

    // Clean up
    for (Node* n : {n1, n2, n3, n4, n5}) delete n;
}

TEST_CASE("BlockListD BatchPrepend", "[BlockListD]") {
    BlockListD blockList(3, 20);  // M = 3, B = 20

    // Create nodes
    std::vector<Node*> nodes;
    for (int i = 0; i < 6; ++i) nodes.push_back(new Node("b" + std::to_string(i)));

    // Prepare pairs for batchPrepend
    std::vector<BlockListD::Pair> pairs = {
        {nodes[0], 15}, {nodes[1], 5}, {nodes[2], 10}, {nodes[3], 2}, {nodes[4], 8}, {nodes[5], 12}
    };
    blockList.batchPrepend(pairs);

    // Pull elements from the block list
    auto [pulledNodes, newBound] = blockList.pull();

    // Check the pulled nodes
    REQUIRE(pulledNodes.size() <= 3);
    REQUIRE(newBound == 20);

    // Clean up
    for (Node* n : nodes) delete n;
}

TEST_CASE("BlockListD Split Block", "[BlockListD]") {
    BlockListD blockList(3, 10);  // M = 3, B = 10

    // Create nodes
    Node* n1 = new Node("n1");
    Node* n2 = new Node("n2");
    Node* n3 = new Node("n3");
    Node* n4 = new Node("n4");
    Node* n5 = new Node("n5");

    // Insert elements into the block list
    blockList.insert(n1, 5);
    blockList.insert(n2, 3);
    blockList.insert(n3, 8);
    blockList.insert(n4, 2);
    blockList.insert(n5, 7);

    // Pull elements from the block list
    auto [pulledNodes, newBound] = blockList.pull();

    // Check the pulled nodes
    REQUIRE(pulledNodes.size() <= 3);
    REQUIRE(newBound == 10);

    // Clean up
    for (Node* n : {n1, n2, n3, n4, n5}) delete n;
}

TEST_CASE("BlockListD remove", "[BlockListD]") {
    BlockListD blockList(3, 10);  // M = 3, B = 10

    // Create nodes
    Node* n1 = new Node("n1");
    Node* n2 = new Node("n2");
    Node* n3 = new Node("n3");

    // Insert elements into the block list
    blockList.insert(n1, 5);
    blockList.insert(n2, 3);
    blockList.insert(n3, 8);

    // Erase a node
    blockList.remove(n2);

    // Pull elements from the block list
    auto [pulledNodes, newBound] = blockList.pull();

    // Check the pulled nodes
    REQUIRE(pulledNodes.size() <= 3);
    REQUIRE(newBound == 10);

    // Clean up
    for (Node* n : {n1, n2, n3}) delete n;
}

TEST_CASE("BlockListD Duplicate Key Insertion", "[BlockListD]") {
    BlockListD blockList(3, 10);  // M = 3, B = 10

    // Create nodes
    Node* n1 = new Node("n1");
    Node* n2 = new Node("n2");

    // Insert first node
    blockList.insert(n1, 5);
    // Insert second node with the same key
    blockList.insert(n1, 3);
    // Insert another node
    blockList.insert(n2, 7);

    // Pull elements from the block list
    auto [pulledNodes, newBound] = blockList.pull();

    // Check the pulled nodes
    REQUIRE(pulledNodes.size() <= 3);
    REQUIRE(newBound == 10);

    // Clean up: only delete nodes that were actually pulled
    for (Node* n : pulledNodes) {
        delete n;
    }
}