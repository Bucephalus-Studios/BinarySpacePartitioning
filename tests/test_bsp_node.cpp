#include <gtest/gtest.h>
#include "test_helpers.hpp"
#include "../classes/BSP_Tree/classes/BSP_Node/BSP_Node.hpp"

/**
 * Test suite for BSP_Node class
 */
class BSP_NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    // Helper to create nodes
    std::unique_ptr<BSP_Node> createNode(const BSP_Rectangle& rect, int id = 0) {
        return std::make_unique<BSP_Node>(rect, id);
    }
};

// Test node construction
TEST_F(BSP_NodeTest, NodeConstruction) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect, 42);

    EXPECT_EQ(node->getId(), 42);
    EXPECT_EQ(node->rectangle.x, 0);
    EXPECT_EQ(node->rectangle.y, 0);
    EXPECT_EQ(node->rectangle.width, 100);
    EXPECT_EQ(node->rectangle.height, 100);
}

// Test isLeaf for new node
TEST_F(BSP_NodeTest, IsLeafForNewNode) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    EXPECT_TRUE(node->isLeaf());
}

// Test isLeaf after split
TEST_F(BSP_NodeTest, IsLeafAfterSplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    node->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    EXPECT_FALSE(node->isLeaf());
}

// Test canSplit with sufficient size
TEST_F(BSP_NodeTest, CanSplitSufficientSize) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    EXPECT_TRUE(node->canSplit(10));
    EXPECT_TRUE(node->canSplit(25));
    EXPECT_TRUE(node->canSplit(49));
}

// Test canSplit with insufficient size
TEST_F(BSP_NodeTest, CanSplitInsufficientSize) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    EXPECT_FALSE(node->canSplit(51));  // 100 < 51*2
    EXPECT_FALSE(node->canSplit(100)); // 100 < 100*2
    EXPECT_FALSE(node->canSplit(200)); // 100 < 200*2
}

// Test canSplit boundary cases
TEST_F(BSP_NodeTest, CanSplitBoundary) {
    BSP_Rectangle rect(0, 0, 20, 20);
    auto node = createNode(rect);

    EXPECT_TRUE(node->canSplit(10));   // 20 >= 10*2 (exactly at boundary)
    EXPECT_FALSE(node->canSplit(11));  // 20 < 11*2
}

// Test horizontal split
TEST_F(BSP_NodeTest, HorizontalSplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    bool result = node->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    EXPECT_TRUE(result);
    EXPECT_FALSE(node->isLeaf());
    EXPECT_NE(node->leftChild, nullptr);
    EXPECT_NE(node->rightChild, nullptr);

    // Verify children have correct dimensions
    EXPECT_EQ(node->leftChild->rectangle.y, 0);
    EXPECT_EQ(node->leftChild->rectangle.height, 100);

    EXPECT_EQ(node->rightChild->rectangle.y, 0);
    EXPECT_EQ(node->rightChild->rectangle.height, 100);

    // Verify combined width equals parent width
    int combinedWidth = node->leftChild->rectangle.width + node->rightChild->rectangle.width;
    EXPECT_EQ(combinedWidth, 100);
}

// Test vertical split
TEST_F(BSP_NodeTest, VerticalSplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    bool result = node->split(BSP_SplitDirection::Vertical, 10, nodeCreator);

    EXPECT_TRUE(result);
    EXPECT_FALSE(node->isLeaf());
    EXPECT_NE(node->leftChild, nullptr);
    EXPECT_NE(node->rightChild, nullptr);

    // Verify children have correct dimensions
    EXPECT_EQ(node->leftChild->rectangle.x, 0);
    EXPECT_EQ(node->leftChild->rectangle.width, 100);

    EXPECT_EQ(node->rightChild->rectangle.x, 0);
    EXPECT_EQ(node->rightChild->rectangle.width, 100);

    // Verify combined height equals parent height
    int combinedHeight = node->leftChild->rectangle.height + node->rightChild->rectangle.height;
    EXPECT_EQ(combinedHeight, 100);
}

// Test split fails when node is too small
TEST_F(BSP_NodeTest, SplitFailsTooSmall) {
    BSP_Rectangle rect(0, 0, 15, 15);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    bool result = node->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);

    EXPECT_FALSE(result);
    EXPECT_TRUE(node->isLeaf());
}

// Test split fails when node already has children
TEST_F(BSP_NodeTest, SplitFailsAlreadySplit) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    // First split should succeed
    bool firstSplit = node->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);
    EXPECT_TRUE(firstSplit);

    // Second split should fail
    bool secondSplit = node->split(BSP_SplitDirection::Vertical, 10, nodeCreator);
    EXPECT_FALSE(secondSplit);
}

// Test split maintains minimum size constraints
TEST_F(BSP_NodeTest, SplitRespectsMinimumSize) {
    BSP_Rectangle rect(0, 0, 100, 100);
    auto node = createNode(rect);

    int nextId = 1;
    auto nodeCreator = [&nextId](const BSP_Rectangle& r) {
        return std::make_unique<BSP_Node>(r, nextId++);
    };

    int minSize = 25;
    bool result = node->split(BSP_SplitDirection::Horizontal, minSize, nodeCreator);

    EXPECT_TRUE(result);

    // Both children should be at least minSize wide
    EXPECT_GE(node->leftChild->rectangle.width, minSize);
    EXPECT_GE(node->rightChild->rectangle.width, minSize);
}
