#pragma once
#include <random>
#include <algorithm>

/**
 * @brief Mock implementation of stevensMathLib for benchmarking purposes
 *
 * This provides the random number generation functionality needed by the BSP library.
 */
namespace stevensMathLib
{
    /**
     * @brief Generates a random integer between min (inclusive) and max (exclusive)
     *
     * @param min The minimum value (inclusive)
     * @param max The maximum value (exclusive)
     * @return A random integer in the range [min, max)
     */
    inline int randomInt(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max - 1);
        return dis(gen);
    }
}
