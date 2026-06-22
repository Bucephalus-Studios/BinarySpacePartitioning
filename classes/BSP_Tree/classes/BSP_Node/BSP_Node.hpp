#include "classes/BSP_Rectangle.hpp"
#include "classes/BSP_SplitDirection.hpp"
#include <stevensMathLib.h>
#include <memory>
#include <tuple>
#include <cmath>
#include <functional>

#pragma once

/**
 * @brief This class represents a node in the Binary Space Partitioning (BSP) tree.
 */
class BSP_Node
{
public:
    /***** MEMBER VARIABLES *****/
    BSP_Rectangle rectangle;
    std::unique_ptr<BSP_Node> leftChild;
    std::unique_ptr<BSP_Node> rightChild;
    int id;


    /***** CONSTRUCTORS *****/
    //Default constructor
    BSP_Node(   const BSP_Rectangle & rect, const int idParam   )
                : rectangle(rect), id(idParam) {};


    /***** METHODS *****/
    /**
     * @brief Checks if this node is a leaf node, meaning it has no children.
     * 
     * @return true if this node is a leaf, false otherwise.
     */
    bool isLeaf() const 
    {
        return leftChild == nullptr && rightChild == nullptr;
    }


    /**
     * @brief Retrieves this BSP_Node's id
     */
    int getId() const
    {
        return id;
    }


    /**
     * @brief Checks to see if this BSP_Node can be split.
     * 
     * @param minimumSplitSize The minimum size required for each dimension after splitting.
     * @return True if the node can be split while maintaining minimum size requirements.
     */
    bool canSplit(int minimumSplitSize)
    {
        return rectangle.width >= (minimumSplitSize * 2) && rectangle.height >= (minimumSplitSize * 2);
    }


    /**
     * @brief Split this BSP_Node into two child nodes based on the specified direction.
     * 
     * @param direction The direction to split the node in (BSP_SplitDirection::Horizontal or BSP_SplitDirection::Vertical).
     * @param minimumSplitSize The minimum size for splitting.
     * @param nodeCreator A function that creates nodes with unique IDs.
     * 
     * @return True if the split was successful, false otherwise.
     */
    bool split( BSP_SplitDirection direction,
                int minimumSplitSize,
                std::function<std::unique_ptr<BSP_Node>(const BSP_Rectangle&)> nodeCreator )
    {
        //Check if the node is a leaf, as we can only split leaf nodes
        if( !isLeaf() )
        {
            return false;
        }

        //Check to see if the room is too small to split
        if( rectangle.width < (minimumSplitSize * 2) || rectangle.height < (minimumSplitSize * 2) )
        {
            return false;
        }

        //Perform the split based on the direction
        switch( direction )
        {
            case BSP_SplitDirection::Horizontal:
            {
                int splitPos = stevensMathLib::randomInt(minimumSplitSize, rectangle.width - minimumSplitSize);
                BSP_Rectangle leftRect(rectangle.x, rectangle.y, splitPos, rectangle.height);
                BSP_Rectangle rightRect(rectangle.x + splitPos, rectangle.y, rectangle.width - splitPos, rectangle.height);
                leftChild = nodeCreator(leftRect);
                rightChild = nodeCreator(rightRect);
                break;
            }
            case BSP_SplitDirection::Vertical:
            {
                int splitPos = stevensMathLib::randomInt(minimumSplitSize, rectangle.height - minimumSplitSize);
                BSP_Rectangle topRect(rectangle.x, rectangle.y, rectangle.width, splitPos);
                BSP_Rectangle bottomRect(rectangle.x, rectangle.y + splitPos, rectangle.width, rectangle.height - splitPos);
                leftChild = nodeCreator(topRect);
                rightChild = nodeCreator(bottomRect);
                break;
            }
        }

        return true;
    }

};