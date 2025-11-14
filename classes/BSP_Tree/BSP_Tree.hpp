#include "classes/BSP_Node/BSP_Node.hpp"
#include <vector>
#include <algorithm>
#include <random>

#pragma once

/**
 * @brief A class representing a Binary Space Partitioning (BSP) tree.
 */
class BSP_Tree
{
public:
    /***** MEMBER VARIABLES *****/
    std::unique_ptr<BSP_Node> root; // The root node of the BSP tree. It points to the rest of the tree!

private:
    int nextNodeId = 0; // Counter for assigning unique IDs to nodes

public:


    /***** CONSTRUCTORS *****/
    //Default constructor
    BSP_Tree() : root(nullptr) {}


    /***** METHODS *****/
    /**
     * @brief Gets the root node of the BSP tree.
     */
    BSP_Node* getRoot() const
    {
        return root.get();
    }


    /**
     * @brief Sets the root node of the BSP tree.
     */
    void setRoot(BSP_Node* newRoot)
    {
        root.reset(newRoot);
    }


    /**
     * @brief Creates a new BSP_Node with a unique ID.
     * 
     * @param rectangle The rectangle for the new node.
     * @return A unique_ptr to the newly created node.
     */
    std::unique_ptr<BSP_Node> createNode(const BSP_Rectangle& rectangle)
    {
        return std::make_unique<BSP_Node>(rectangle, nextNodeId++);
    }


    /**
     * @brief Gets the next available node ID without incrementing it.
     * 
     * @return The next ID that will be assigned to a node.
     */
    int getNextNodeId() const
    {
        return nextNodeId;
    }


    /**
     * @brief Gets the total number of nodes currently in the tree.
     * 
     * @return The total count of nodes in the tree.
     */
    int getNodeCount() const
    {
        return countNodesRecursive(root.get());
    }


    /**
     * @brief Gets a vector of all of hte leaf nodes from the BSP tree.
     */
    std::vector<BSP_Node*> getLeafNodes() const
    {
        std::vector<BSP_Node*> leafNodes;
        getLeafNodesRecursive(root.get(), leafNodes);
        return leafNodes;
    }


    /**
     * @brief Gets a vector of random leaf nodes from the BSP tree.
     * 
     * @param count The number of random leaf nodes to return.
     * 
     * @return A vector of pointers to random leaf nodes.
     */
    std::vector<BSP_Node*> getRandomLeafNodes(int count) const
    {
        //Get all of the leaf nodes in the BSP tree
        std::vector<BSP_Node*> leafNodes = getLeafNodes();

        //Randomly shuffle the leaf nodes and return the first 'count' nodes
        std::shuffle(leafNodes.begin(), leafNodes.end(), std::mt19937{ std::random_device{}() });
        leafNodes.resize(std::min(count, static_cast<int>(leafNodes.size())));
        return leafNodes;
    }


private:
    /**
     * @brief Recursively collects all leaf nodes from the BSP tree.
     * 
     * @param node The current node to check.
     * @param leafNodes The vector to store leaf nodes in.
     */
    void getLeafNodesRecursive(BSP_Node* node, std::vector<BSP_Node*>& leafNodes) const
    {
        if (node == nullptr)
        {
            return;
        }
        
        if (node->isLeaf())
        {
            leafNodes.push_back(node);
        } 
        else
        {
            getLeafNodesRecursive(node->leftChild.get(), leafNodes);
            getLeafNodesRecursive(node->rightChild.get(), leafNodes);
        }
    }

    /**
     * @brief Recursively counts all nodes in the tree.
     * 
     * @param node The current node to count from.
     * @return The total number of nodes in the subtree rooted at node.
     */
    int countNodesRecursive(BSP_Node* node) const
    {
        if (node == nullptr) {
            return 0;
        }
        
        return 1 + countNodesRecursive(node->leftChild.get()) + countNodesRecursive(node->rightChild.get());
    }
};