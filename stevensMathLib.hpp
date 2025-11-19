#pragma once
#include <random>
#include <chrono>

/**
 * @brief Math utility library providing random number generation
 *
 * This library provides essential mathematical utilities for the Binary Space
 * Partitioning library. Currently focuses on random number generation.
 */
namespace stevensMathLib
{
    /**
     * @brief Generates a random integer between min (inclusive) and max (exclusive)
     *
     * Uses thread-local Mersenne Twister generator seeded with high-resolution clock
     * for thread-safe random number generation with good distribution properties.
     *
     * @param min The minimum value (inclusive)
     * @param max The maximum value (exclusive)
     * @return A random integer in the range [min, max)
     *
     * @note If min >= max, returns min to handle edge cases gracefully
     * @note Uses thread_local storage to ensure thread safety
     */
    inline int randomInt(int min, int max)
    {
        // Handle edge case where min >= max
        // This can occur during BSP splitting when rectangle dimensions
        // are exactly minimumSplitSize * 2
        if (min >= max) {
            return min;
        }

        // Use thread_local to ensure thread-safe random number generation
        // Each thread gets its own generator instance
        thread_local std::mt19937 gen(
            static_cast<unsigned int>(
                std::chrono::steady_clock::now().time_since_epoch().count()
            )
        );

        // Generate random integer in range [min, max-1]
        std::uniform_int_distribution<> dis(min, max - 1);
        return dis(gen);
    }
}
