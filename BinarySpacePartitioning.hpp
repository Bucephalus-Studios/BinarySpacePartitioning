#include "classes/BSP_Tree/BSP_Tree.hpp"
#include "BSP_Settings.hpp"

#pragma once


/**
 * @brief A library for Binary Space Partitioning (BSP) in C++.
 * 
 * @param width 
 * @param height
 * @param minimumSplitSize The minimum size of a split in the BSP tree.
 * 
 * This library provides classes and functions for creating and manipulating BSP trees.
 */
namespace BinarySpacePartitioning
{


    /**
     * @brief Given the dimensions of a 2D rectangular space and a minimum size for splitting, this function runs the 
     *        Binary Space Partitioning algorithm to create a BSP tree for the described space.
     * 
     * @param width The width of the rectangular space to partition.
     * @param height The height of the rectangular space to partition.
     * @param minimumSplitSize The minimum size of a split in the BSP tree.
     * @param requiredPartitions The number of partitions required for the BSP tree to split into. BSP may split more than this number, but
     *                           this is the minimum number of partitions this function will generate.
     * @param settings BSP configuration settings (optional, defaults to geometric centers)
     * 
     * @return A BSP_Tree object representing the partitioned space.
     */
    BSP_Tree runBSP(    int width,
                        int height,
                        int minimumSplitSize,
                        int requiredPartitions,
                        const BSP_Settings& settings = BSP_Settings() )
    {
        // Calculate appropriate padding based on minimum split size
        BSP_Settings adjustedSettings = settings;
        if (adjustedSettings.randomPartitionCenters) {
            adjustedSettings.centerPadding = BSP_Settings::calculatePaddingForMinSize(minimumSplitSize);
        }
        //Create the initial rectangle that represents the entire space
        BSP_Rectangle initialRectangle( 0, 0, width, height );
        
        // If using random centers, set a random center for the initial rectangle
        if (adjustedSettings.randomPartitionCenters) {
            // Use configurable padding that's appropriate for the minimum split size
            int padding = adjustedSettings.centerPadding;
            // Ensure we have enough space for padding
            if (width > 2 * padding && height > 2 * padding) {
                int centerX = stevensMathLib::randomInt(padding, width - padding);
                int centerY = stevensMathLib::randomInt(padding, height - padding);
                initialRectangle.center = std::make_tuple(centerX, centerY);
            }
            // Otherwise, keep the geometric center that was already calculated in BSP_Rectangle constructor
        }

        //Declare the default BSP_Tree  
        BSP_Tree bspTree;

        //Run the BSP algorithm to create the BSP tree
        bspTree.root = bspTree.createNode(initialRectangle);

        // Calculate required depth to get approximately the desired number of partitions
        int requiredDepth = 0;
        int partitions = 1;
        while (partitions < requiredPartitions)
        {
            requiredDepth++;
            partitions *= 2;
        }

        // Collect nodes to split at each level, starting with the root node
        std::vector<BSP_Node*> currentLevelNodes = {bspTree.root.get()};
        //Split the bspTree's leaf nodes until the required depth is reached or we run out of nodes to split
        for (int depth = 0; depth < requiredDepth && !currentLevelNodes.empty(); depth++) 
        {
            std::vector<BSP_Node*> nextLevelNodes;
        
            for (BSP_Node* node : currentLevelNodes) 
            {
                if ( node->isLeaf() && node->canSplit(minimumSplitSize) ) 
                {
                    // Choose split direction randomly for better room distribution
                    BSP_SplitDirection direction = stevensMathLib::randomInt(0, 2) == 0 ? 
                        BSP_SplitDirection::Horizontal : BSP_SplitDirection::Vertical;
                    
                    // Attempt to split the node
                    auto nodeCreator = [&bspTree, &adjustedSettings](const BSP_Rectangle& rect) {
                        BSP_Rectangle newRect = rect;
                        // If using random centers, generate random center for child rectangle
                        if (adjustedSettings.randomPartitionCenters) {
                            // Use configurable padding that's appropriate for the minimum split size
                            int padding = adjustedSettings.centerPadding;
                            // Ensure we have enough space for padding, fallback to geometric center if not
                            if (rect.width > 2 * padding && rect.height > 2 * padding) {
                                int centerX = stevensMathLib::randomInt(rect.x + padding, rect.x + rect.width - padding);
                                int centerY = stevensMathLib::randomInt(rect.y + padding, rect.y + rect.height - padding);
                                newRect.center = std::make_tuple(centerX, centerY);
                            }
                            // Otherwise, keep the geometric center that was already calculated in BSP_Rectangle constructor
                        }
                        return bspTree.createNode(newRect);
                    };
                    if (node->split(direction, minimumSplitSize, nodeCreator))
                    {
                        // Add child nodes to next level for further splitting
                        if (node->leftChild)
                        {
                            nextLevelNodes.push_back(node->leftChild.get());
                        }
                        if (node->rightChild)
                        {
                            nextLevelNodes.push_back(node->rightChild.get());
                        }
                    }
                }
            }
            // Move to the next level of nodes to split
            currentLevelNodes = nextLevelNodes;
        }

        return bspTree;
    }


}