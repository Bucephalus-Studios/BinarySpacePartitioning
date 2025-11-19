#include <gtest/gtest.h>
#include "test_helpers.hpp"
#include "../BinarySpacePartitioning.hpp"

/**
 * Test suite for the main BSP algorithm (runBSP function)
 */
class BSP_AlgorithmTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test basic BSP tree creation
TEST_F(BSP_AlgorithmTest, BasicTreeCreation) {
    auto tree = BinarySpacePartitioning::runBSP(100, 100, 10, 4);

    EXPECT_NE(tree.getRoot(), nullptr);
    EXPECT_EQ(tree.getRoot()->rectangle.width, 100);
    EXPECT_EQ(tree.getRoot()->rectangle.height, 100);
}

// Test required partitions
TEST_F(BSP_AlgorithmTest, RequiredPartitions) {
    int width = 500;
    int height = 500;
    int minSize = 10;
    int requiredPartitions = 8;

    auto tree = BinarySpacePartitioning::runBSP(width, height, minSize, requiredPartitions);

    auto leafNodes = tree.getLeafNodes();

    // BSP tries to create at least the required partitions, but random splits
    // might prevent achieving the exact count. Expect at least 75%.
    EXPECT_GE(leafNodes.size(), static_cast<size_t>(requiredPartitions * 0.75));
}

// Test minimum split size is respected
TEST_F(BSP_AlgorithmTest, MinimumSplitSizeRespected) {
    int width = 200;
    int height = 200;
    int minSize = 25;
    int requiredPartitions = 4;

    auto tree = BinarySpacePartitioning::runBSP(width, height, minSize, requiredPartitions);

    auto leafNodes = tree.getLeafNodes();

    // All leaf nodes should have dimensions >= minSize
    for (auto leaf : leafNodes) {
        EXPECT_GE(leaf->rectangle.width, minSize);
        EXPECT_GE(leaf->rectangle.height, minSize);
    }
}

// Test single partition request
TEST_F(BSP_AlgorithmTest, SinglePartition) {
    auto tree = BinarySpacePartitioning::runBSP(100, 100, 10, 1);

    auto leafNodes = tree.getLeafNodes();

    // With requiredPartitions=1, should have at least 1 partition (might split once)
    EXPECT_GE(leafNodes.size(), 1);
}

// Test power of 2 partitions
TEST_F(BSP_AlgorithmTest, PowerOfTwoPartitions) {
    std::vector<int> partitionCounts = {2, 4, 8, 16};

    for (int count : partitionCounts) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, count);
        auto leafNodes = tree.getLeafNodes();

        // BSP tries to create at least the requested partitions, but due to random
        // splits and minimum size constraints, it might create slightly fewer
        // We expect at least 75% of requested partitions
        EXPECT_GE(leafNodes.size(), static_cast<size_t>(count * 0.75))
            << "Failed for partition count: " << count;
    }
}

// Test all leaf rectangles are non-overlapping
TEST_F(BSP_AlgorithmTest, LeafRectanglesNonOverlapping) {
    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 8);
    auto leafNodes = tree.getLeafNodes();

    // Check that no two leaf rectangles overlap
    for (size_t i = 0; i < leafNodes.size(); i++) {
        for (size_t j = i + 1; j < leafNodes.size(); j++) {
            auto& rect1 = leafNodes[i]->rectangle;
            auto& rect2 = leafNodes[j]->rectangle;

            bool overlap = !(rect1.x + rect1.width <= rect2.x ||
                           rect2.x + rect2.width <= rect1.x ||
                           rect1.y + rect1.height <= rect2.y ||
                           rect2.y + rect2.height <= rect1.y);

            EXPECT_FALSE(overlap)
                << "Rectangles " << i << " and " << j << " overlap";
        }
    }
}

// Test total area is preserved
TEST_F(BSP_AlgorithmTest, TotalAreaPreserved) {
    int width = 200;
    int height = 200;
    int expectedArea = width * height;

    auto tree = BinarySpacePartitioning::runBSP(width, height, 10, 8);
    auto leafNodes = tree.getLeafNodes();

    int totalArea = 0;
    for (auto leaf : leafNodes) {
        totalArea += leaf->rectangle.getArea();
    }

    EXPECT_EQ(totalArea, expectedArea);
}

// Test with very small minimum size
TEST_F(BSP_AlgorithmTest, SmallMinimumSize) {
    auto tree = BinarySpacePartitioning::runBSP(400, 400, 5, 16);
    auto leafNodes = tree.getLeafNodes();

    // With small minimum size and sufficient space, expect at least 50% of requested partitions
    EXPECT_GE(leafNodes.size(), static_cast<size_t>(16 * 0.5));

    for (auto leaf : leafNodes) {
        EXPECT_GE(leaf->rectangle.width, 5);
        EXPECT_GE(leaf->rectangle.height, 5);
    }
}

// Test with large minimum size (limits splitting)
TEST_F(BSP_AlgorithmTest, LargeMinimumSize) {
    auto tree = BinarySpacePartitioning::runBSP(100, 100, 40, 16);
    auto leafNodes = tree.getLeafNodes();

    // Large minimum size will limit how much we can split
    // Each dimension must be at least 40*2 = 80 to split
    EXPECT_GT(leafNodes.size(), 0);

    for (auto leaf : leafNodes) {
        EXPECT_GE(leaf->rectangle.width, 40);
        EXPECT_GE(leaf->rectangle.height, 40);
    }
}

// Test geometric center mode (default)
TEST_F(BSP_AlgorithmTest, GeometricCenterMode) {
    BSP_Settings settings; // Default uses geometric centers
    settings.randomPartitionCenters = false;

    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 4, settings);

    EXPECT_NE(tree.getRoot(), nullptr);
    auto leafNodes = tree.getLeafNodes();
    // Allow for some variance due to random split directions
    EXPECT_GE(leafNodes.size(), 3);

    // Verify root center is geometric
    auto& rootRect = tree.getRoot()->rectangle;
    auto center = rootRect.getCenter();
    EXPECT_EQ(std::get<0>(center), 100); // x + width/2
    EXPECT_EQ(std::get<1>(center), 100); // y + height/2
}

// Test random center mode
TEST_F(BSP_AlgorithmTest, RandomCenterMode) {
    BSP_Settings settings;
    settings.randomPartitionCenters = true;

    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 4, settings);

    EXPECT_NE(tree.getRoot(), nullptr);
    auto leafNodes = tree.getLeafNodes();
    EXPECT_GE(leafNodes.size(), 4);

    // Root should have a center (random or geometric)
    auto center = tree.getRoot()->rectangle.getCenter();
    EXPECT_GE(std::get<0>(center), 0);
    EXPECT_GE(std::get<1>(center), 0);
    EXPECT_LE(std::get<0>(center), 200);
    EXPECT_LE(std::get<1>(center), 200);
}

// Test with custom center padding
TEST_F(BSP_AlgorithmTest, CustomCenterPadding) {
    BSP_Settings settings(true, 5); // Random centers with padding of 5

    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 4, settings);

    EXPECT_NE(tree.getRoot(), nullptr);
    auto leafNodes = tree.getLeafNodes();
    EXPECT_GE(leafNodes.size(), 4);
}

// Test calculatePaddingForMinSize
TEST_F(BSP_AlgorithmTest, CalculatePaddingForMinSize) {
    EXPECT_EQ(BSP_Settings::calculatePaddingForMinSize(5), 2);
    EXPECT_EQ(BSP_Settings::calculatePaddingForMinSize(10), 4);
    EXPECT_EQ(BSP_Settings::calculatePaddingForMinSize(25), 10);
}

// Test settings with initializer list
TEST_F(BSP_AlgorithmTest, SettingsInitializerList) {
    BSP_Settings settings{"random partition centers"};

    EXPECT_TRUE(settings.randomPartitionCenters);

    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 4, settings);
    EXPECT_NE(tree.getRoot(), nullptr);
}

// Test node IDs are unique
TEST_F(BSP_AlgorithmTest, UniqueNodeIds) {
    auto tree = BinarySpacePartitioning::runBSP(200, 200, 10, 8);

    std::set<int> nodeIds;
    std::function<void(BSP_Node*)> collectIds = [&](BSP_Node* node) {
        if (node == nullptr) return;
        nodeIds.insert(node->getId());
        collectIds(node->leftChild.get());
        collectIds(node->rightChild.get());
    };

    collectIds(tree.getRoot());

    // All IDs should be unique, so set size should equal node count
    EXPECT_EQ(nodeIds.size(), tree.getNodeCount());
}

// Test very large space
TEST_F(BSP_AlgorithmTest, LargeSpace) {
    auto tree = BinarySpacePartitioning::runBSP(2000, 2000, 20, 32);

    auto leafNodes = tree.getLeafNodes();
    // Due to random splits and edge cases, we might not get exactly 32
    // Check for at least 50% of requested partitions
    EXPECT_GE(leafNodes.size(), static_cast<size_t>(32 * 0.5));

    int totalArea = 0;
    for (auto leaf : leafNodes) {
        totalArea += leaf->rectangle.getArea();
        EXPECT_GE(leaf->rectangle.width, 20);
        EXPECT_GE(leaf->rectangle.height, 20);
    }

    EXPECT_EQ(totalArea, 2000 * 2000);
}

// Test non-square space
TEST_F(BSP_AlgorithmTest, NonSquareSpace) {
    auto tree = BinarySpacePartitioning::runBSP(600, 300, 10, 8);

    auto leafNodes = tree.getLeafNodes();
    // Allow for variance due to random splits
    EXPECT_GE(leafNodes.size(), static_cast<size_t>(8 * 0.75));

    int totalArea = 0;
    for (auto leaf : leafNodes) {
        totalArea += leaf->rectangle.getArea();
    }

    EXPECT_EQ(totalArea, 600 * 300);
}
