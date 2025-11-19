#pragma once
#include <random>
#include <algorithm>
#include <chrono>

/**
 * @brief Mock implementation of stevensMathLib for testing purposes
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
        // Handle edge case where min >= max
        if (min >= max) {
            return min;
        }
        // Use thread_local to avoid static initialization issues
        thread_local std::mt19937 gen(
            static_cast<unsigned int>(
                std::chrono::steady_clock::now().time_since_epoch().count()
            )
        );
        std::uniform_int_distribution<> dis(min, max - 1);
        return dis(gen);
    }
}
