//Describes the different directions a BSP node can be split
//This is used in the Binary Space Partitioning algorithm to determine how to split a node into two child nodes.

#pragma once

enum class BSP_SplitDirection
{
    Horizontal,
    Vertical
};