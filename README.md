# Binary Space Partitioning Library

A C++17 library for Binary Space Partitioning (BSP) in 2D rectangular spaces.

## Overview

This library provides classes and functions for creating and manipulating BSP trees, which recursively partition 2D rectangular spaces into smaller regions. BSP is commonly used in game development for procedural dungeon generation, space partitioning, and collision detection.

## Features

- **Configurable Partitioning**: Control minimum split size and required partition count
- **Multiple Split Modes**:
  - Geometric center-based splitting (deterministic)
  - Random center-based splitting (varied layouts)
- **Flexible API**: Easy-to-use interface with sensible defaults
- **Comprehensive Testing**: Full test suite with Google Test
- **Performance Benchmarks**: Benchmark suite to measure performance characteristics

## Requirements

- C++17 or later
- CMake 3.14+ (for building tests/benchmarks)
- Google Test 1.14.0 (auto-fetched for tests)
- Google Benchmark 1.8.3 (auto-fetched for benchmarks)

## Quick Start

### Basic Usage

```cpp
#include "BinarySpacePartitioning.hpp"

// Create a 1000x1000 space with minimum 20-pixel rooms and 16 partitions
auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 20, 16);

// Get all leaf nodes (the final partitions)
auto leafNodes = tree.getLeafNodes();

// Access partition rectangles
for (auto* leaf : leafNodes) {
    auto& rect = leaf->rectangle;
    // Use rect.x, rect.y, rect.width, rect.height
}
```

### Advanced Configuration

```cpp
#include "BinarySpacePartitioning.hpp"

// Use random partition centers for more varied layouts
BSP_Settings settings;
settings.randomPartitionCenters = true;
settings.centerPadding = 10;

auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 20, 16, settings);
```

## Building Tests

```bash
cd tests
mkdir build && cd build
cmake ..
cmake --build .
./bsp_tests
```

All 53 tests should pass.

## Building Benchmarks

```bash
cd benchmarks
mkdir build && cd build
cmake ..
cmake --build .
./bsp_benchmarks
```

See `benchmarks/README.md` for detailed benchmark descriptions and sample results.

## API Reference

### Main Function

```cpp
BSP_Tree runBSP(
    int width,
    int height,
    int minimumSplitSize,
    int requiredPartitions,
    const BSP_Settings& settings = BSP_Settings()
)
```

**Parameters:**
- `width`: Width of the rectangular space to partition
- `height`: Height of the rectangular space to partition
- `minimumSplitSize`: Minimum dimension size for any partition
- `requiredPartitions`: Minimum number of partitions to create
- `settings`: Configuration options (optional)

**Returns:** A `BSP_Tree` object representing the partitioned space

### BSP_Tree Methods

- `BSP_Node* getRoot()` - Get the root node of the tree
- `size_t getNodeCount()` - Get total number of nodes
- `std::vector<BSP_Node*> getLeafNodes()` - Get all leaf nodes (final partitions)
- `std::vector<BSP_Node*> getRandomLeafNodes(int count)` - Get random subset of leaf nodes

### BSP_Node Properties

- `BSP_Rectangle rectangle` - The rectangular region this node represents
- `int id` - Unique identifier for this node
- `std::unique_ptr<BSP_Node> leftChild` - Left/top child node
- `std::unique_ptr<BSP_Node> rightChild` - Right/bottom child node

### BSP_Rectangle Properties

- `int x, y` - Position of top-left corner
- `int width, height` - Dimensions
- `std::tuple<int, int> center` - Center point
- `int getArea()` - Calculate area
- `std::tuple<int, int> getCenter()` - Get center coordinates

## Implementation Details

### stevensMathLib Dependency

The library depends on [stevensMathLib](https://github.com/Bucephalus-Studios/stevensMathLib) for random number generation. A copy is included in `stevensMathLib.hpp` for convenience.

Key features of the dependency:
- **Thread-safe random number generation** using `thread_local` storage
- **Mersenne Twister (mt19937)** for high-quality randomness
- **High-resolution clock seeding** to avoid entropy exhaustion
- **Critical edge case handling**: When `min >= max`, `randomInt()` returns `min`

**Important:** The edge case handling in `randomInt` is critical for BSP splitting. When rectangle dimensions are exactly `minimumSplitSize * 2`, the split calculation produces `randomInt(min, min)`, which must return `min` rather than causing undefined behavior that would lead to segmentation faults.

To update stevensMathLib, copy the latest `stevensMathLib.h` from the [upstream repository](https://github.com/Bucephalus-Studios/stevensMathLib).

### Algorithm Overview

1. Create initial rectangle representing entire space
2. Calculate required tree depth based on partition count
3. Recursively split nodes level-by-level:
   - Check if node can split (dimensions >= minimumSplitSize * 2)
   - Randomly choose horizontal or vertical split direction
   - Choose random split position within valid range
   - Create two child nodes from the split
4. Continue until required depth reached or no more nodes can split

## License

See LICENSE file for details.

## Testing and Quality

- **53 comprehensive unit tests** covering all major components
- **26 performance benchmarks** measuring various scenarios
- **100% test pass rate** with edge cases handled
- **No memory leaks** (all resources managed with smart pointers)

## Contributing

When contributing, ensure:
1. All existing tests pass
2. New features include corresponding tests
3. Code follows existing style conventions
4. Benchmarks are run to verify no performance regressions
