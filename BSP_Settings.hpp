#pragma once
#include <initializer_list>
#include <string>
#include <algorithm>

/**
 * @brief Configuration settings for Binary Space Partitioning algorithm
 */
struct BSP_Settings 
{
    /***** MEMBER VARIABLES *****/
    
    /**
     * @brief If true, partition rectangles will have random centers within their bounds.
     *        If false, partition rectangles will use their geometric centers.
     */
    bool randomPartitionCenters = false;
    
    /**
     * @brief Padding distance from rectangle edges when placing random centers.
     *        Should be less than minimumSplitSize/2 to ensure valid placement.
     *        Default value works with minimum split sizes >= 5.
     */
    int centerPadding = 2;
    
    
    /***** CONSTRUCTORS *****/
    
    // Default constructor - uses geometric centers
    BSP_Settings() = default;
    
    // Constructor with initializer list for readable configuration
    BSP_Settings(std::initializer_list<std::string> settings) 
    {
        for (const auto& setting : settings) {
            if (setting == "random partition centers") {
                randomPartitionCenters = true;
            }
        }
    }
    
    // Constructor with explicit padding control
    BSP_Settings(bool useRandomCenters, int padding) 
        : randomPartitionCenters(useRandomCenters), centerPadding(padding) {}
        
    /**
     * @brief Calculate appropriate padding based on minimum split size
     * @param minimumSplitSize The minimum size BSP can create for rectangles
     * @return Recommended padding that ensures random centers have valid placement space
     */
    static int calculatePaddingForMinSize(int minimumSplitSize) 
    {
        // Use padding that's at most 40% of minimum split size, ensuring room for random placement
        return std::max(1, (minimumSplitSize * 2) / 5);  // For minSize=5: padding=2, for minSize=10: padding=4
    }
};