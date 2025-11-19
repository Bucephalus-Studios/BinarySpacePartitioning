#include <gtest/gtest.h>
#include "test_helpers.hpp"
#include "../classes/BSP_Tree/BSP_Tree.hpp"

/**
 * Test suite for BSP_Tree class
 */
class BSP_TreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        tree = std::make_unique<BSP_Tree>();
    }

    std::unique_ptr<BSP_Tree> tree;
};

// Test default construction
TEST_F(BSP_TreeTest, DefaultConstruction) {
    EXPECT_EQ(tree->getRoot(), nullptr);
}

// Test createNode
TEST_F(BSP_TreeTest, CreateNode) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = tree->createNode(rect);

    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->rectangle.width, 100);
    EXPECT_EQ(node->rectangle.height, 100);
    EXPECT_EQ(node->getId(), 0);
}

// Test createNode assigns unique IDs
TEST_F(BSP_TreeTest, CreateNodeUniqueIds) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node1 = tree->createNode(rect);
    auto node2 = tree->createNode(rect);
    auto node3 = tree->createNode(rect);

    EXPECT_EQ(node1->getId(), 0);
    EXPECT_EQ(node2->getId(), 1);
    EXPECT_EQ(node3->getId(), 2);
}

// Test getNextNodeId
TEST_F(BSP_TreeTest, GetNextNodeId) {
    EXPECT_EQ(tree->getNextNodeId(), 0);

    BSP_Rectangle rect(0, 0, 100, 100);
    auto node1 = tree->createNode(rect);
    EXPECT_EQ(tree->getNextNodeId(), 1);

    auto node2 = tree->createNode(rect);
    EXPECT_EQ(tree->getNextNodeId(), 2);
}

// Test setRoot and getRoot
TEST_F(BSP_TreeTest, SetAndGetRoot) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = new BSP_Node(rect, 0);

    tree->setRoot(node);

    EXPECT_NE(tree->getRoot(), nullptr);
    EXPECT_EQ(tree->getRoot()->getId(), 0);
}

// Test getNodeCount with single node
TEST_F(BSP_TreeTest, GetNodeCountSingleNode) {
    BSP_Rectangle rect(0, 0, 100, 100);
    tree->root = tree->createNode(rect);

    EXPECT_EQ(tree->getNodeCount(), 1);
}

// Test getNodeCount with split tree
TEST_F(BSP_TreeTest, GetNodeCountAfterSplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    tree->root = tree->createNode(rect);

    // Split the root
    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    EXPECT_EQ(tree->getNodeCount(), 3); // root + 2 children
}

// Test getNodeCount with multiple splits
TEST_F(BSP_TreeTest, GetNodeCountMultipleSplits) {
    BSP_Rectangle rect(0, 0, 200, 200);
    tree->root = tree->createNode(rect);

    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    // Split root
    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    // Split left child
    tree->root->leftChild->split(BSP_SplitDirection::Vertical, 10, nodeCreator);

    // Split right child
    tree->root->rightChild->split(BSP_SplitDirection::Vertical, 10, nodeCreator);

    EXPECT_EQ(tree->getNodeCount(), 7); // root + 2 children + 4 grandchildren
}

// Test getLeafNodes with single node
TEST_F(BSP_TreeTest, GetLeafNodesSingleNode) {
    BSP_Rectangle rect(0, 0, 100, 100);
    tree->root = tree->createNode(rect);

    auto leafNodes = tree->getLeafNodes();

    EXPECT_EQ(leafNodes.size(), 1);
    EXPECT_EQ(leafNodes[0], tree->root.get());
}

// Test getLeafNodes after single split
TEST_F(BSP_TreeTest, GetLeafNodesAfterSplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    tree->root = tree->createNode(rect);

    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    auto leafNodes = tree->getLeafNodes();

    EXPECT_EQ(leafNodes.size(), 2);
    EXPECT_TRUE(leafNodes[0]->isLeaf());
    EXPECT_TRUE(leafNodes[1]->isLeaf());
}

// Test getLeafNodes with multiple levels
TEST_F(BSP_TreeTest, GetLeafNodesMultipleLevels) {
    BSP_Rectangle rect(0, 0, 200, 200);
    tree->root = tree->createNode(rect);

    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    // Split root
    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    // Split left child
    tree->root->leftChild->split(BSP_SplitDirection::Vertical, 10, nodeCreator);

    auto leafNodes = tree->getLeafNodes();

    EXPECT_EQ(leafNodes.size(), 3); // 2 from left child split + 1 right child
    for (auto leaf : leafNodes) {
        EXPECT_TRUE(leaf->isLeaf());
    }
}

// Test getRandomLeafNodes
TEST_F(BSP_TreeTest, GetRandomLeafNodes) {
    BSP_Rectangle rect(0, 0, 200, 200);
    tree->root = tree->createNode(rect);

    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    // Create multiple leaf nodes with guaranteed successful splits
    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);
    tree->root->leftChild->split(BSP_SplitDirection::Vertical, 10, nodeCreator);
    tree->root->rightChild->split(BSP_SplitDirection::Vertical, 10, nodeCreator);

    // Should have 4 leaf nodes total
    auto allLeaves = tree->getLeafNodes();
    EXPECT_EQ(allLeaves.size(), 4);

    // Get 2 random leaves
    auto randomLeaves = tree->getRandomLeafNodes(2);
    EXPECT_EQ(randomLeaves.size(), 2);

    for (auto leaf : randomLeaves) {
        EXPECT_TRUE(leaf->isLeaf());
    }
}

// Test getRandomLeafNodes with count greater than available
TEST_F(BSP_TreeTest, GetRandomLeafNodesExceedsAvailable) {
    BSP_Rectangle rect(0, 0, 100, 100);
    tree->root = tree->createNode(rect);

    auto nodeCreator = [this](const BSP_Rectangle& r) {
        return tree->createNode(r);
    };

    tree->root->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    // Request more than available (2 leaves available, request 10)
    auto randomLeaves = tree->getRandomLeafNodes(10);

    // Should return all available leaves
    EXPECT_EQ(randomLeaves.size(), 2);
}

// Test getLeafNodes with empty tree
TEST_F(BSP_TreeTest, GetLeafNodesEmptyTree) {
    auto leafNodes = tree->getLeafNodes();
    EXPECT_EQ(leafNodes.size(), 0);
}

// Test getNodeCount with empty tree
TEST_F(BSP_TreeTest, GetNodeCountEmptyTree) {
    EXPECT_EQ(tree->getNodeCount(), 0);
}
