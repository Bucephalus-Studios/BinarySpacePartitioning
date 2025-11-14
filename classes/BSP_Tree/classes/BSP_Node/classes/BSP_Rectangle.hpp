/**
 * @brief This is a member variable of the BSP Node class which denotes a rectangular area stored by a
 *        node in the BSP tree.
 */

#pragma once
#include <tuple>

class BSP_Rectangle
{
public:
    /***** MEMBER VARIABLES *****/
    int x;
    int y;
    int width;
    int height;
    std::tuple<int, int> center;
    

    /***** CONSTRUCTORS *****/
    // Constructor with bounds - calculates geometric center
    BSP_Rectangle(  int xParam = 0, int yParam = 0, int widthParam = 0, int heightParam = 0)
                    : x(xParam), y(yParam), width(widthParam), height(heightParam) 
    {
        // Calculate geometric center of the rectangle
        center = std::make_tuple(x + width / 2, y + height / 2);
    }
    
    // Constructor with bounds and explicit center
    BSP_Rectangle(  int xParam, int yParam, int widthParam, int heightParam, 
                    const std::tuple<int, int>& centerParam)
                    : x(xParam), y(yParam), width(widthParam), height(heightParam), center(centerParam) {}


    /***** METHODS *****/
    //Retrieves the area of the rectangle
    int getArea() const
    {
        return width * height;
    }

    
    //Gets the stored center coordinates of the rectangle
    std::tuple<int,int> getCenter() const
    {
        return center;
    }


    /**
     * @brief Retrieves the width of this BSP_Rectangle.
     */
    int getWidth() const
    {
        return width;
    }


    /**
     * @brief Retrieves the height of this BSP_Rectangle.
     */
    int getHeight() const
    {
        return height;
    }
};