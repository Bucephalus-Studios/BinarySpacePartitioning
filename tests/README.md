# BSP Library Tests

This directory contains comprehensive unit tests for the Binary Space Partitioning library using Google Test.

## Building and Running Tests

```bash
cd tests
cmake -S . -B build
cmake --build build
cd build
./bsp_tests
```

## Test Coverage

### BSP_RectangleTest (10 tests)
- Default and parameterized construction
- Geometric center calculation
- Custom center support
- Area calculations
- Width and height getters
- Zero-sized rectangles

### BSP_NodeTest (11 tests)
- Node construction and ID management
- Leaf node detection
- canSplit functionality with various size constraints
- Horizontal and vertical splitting
- Split failure conditions
- Minimum size respect

### BSP_TreeTest (15 tests)
- Tree construction
- Node creation with unique IDs
- Root management
- Node counting (single and multiple splits)
- Leaf node retrieval
- Random leaf node selection
- Empty tree handling

### BSP_AlgorithmTest (17 tests)
- Basic tree creation
- Required partitions
- Minimum split size enforcement
- Power-of-two partition counts
- Non-overlapping rectangles verification
- Total area preservation
- Small and large minimum sizes
- Geometric vs random center modes
- Custom center padding
- Unique node IDs
- Large spaces and non-square spaces

## Test Results

All 53 tests pass successfully, validating:
- Core data structures (Rectangle, Node, Tree)
- Splitting logic and constraints
- Tree traversal and querying
- BSP algorithm correctness
- Area preservation
- Non-overlapping partitions
- Configuration options

## Dependencies

Tests use Google Test v1.14.0, which is automatically downloaded during CMake configuration.
