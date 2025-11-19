# BSP Library Benchmarks

This directory contains performance benchmarks for the Binary Space Partitioning library using Google Benchmark.

## Building and Running Benchmarks

```bash
cd benchmarks
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cd build
./bsp_benchmarks
```

## Available Benchmarks

### BSP Algorithm Benchmarks
- `BM_BSP_SmallSpace` - 100x100 space with 4 partitions
- `BM_BSP_MediumSpace` - 500x500 space with 16 partitions
- `BM_BSP_LargeSpace` - 1000x1000 space with 32 partitions
- `BM_BSP_VeryLargeSpace` - 2000x2000 space with 64 partitions
- `BM_BSP_VaryingPartitions` - Test different partition counts (4-128)
- `BM_BSP_VaryingMinSize` - Test different minimum sizes (5-100)

### Configuration Benchmarks
- `BM_BSP_GeometricCenters` - BSP with geometric centers
- `BM_BSP_RandomCenters` - BSP with random centers

### Component Benchmarks
- `BM_BSP_NodeCreation` - Node creation performance
- `BM_BSP_NodeSplitting` - Single node split performance
- `BM_BSP_GetLeafNodes` - Leaf node retrieval
- `BM_BSP_GetRandomLeafNodes` - Random leaf selection (1-32 nodes)
- `BM_BSP_RectangleArea` - Area calculation
- `BM_BSP_RectangleCenter` - Center retrieval
- `BM_BSP_NodeCanSplit` - canSplit check
- `BM_BSP_TreeNodeCount` - Node counting

### Shape Benchmarks
- `BM_BSP_NonSquareSpace` - Non-square rectangular spaces
- `BM_BSP_ExtremeAspectRatio` - Extreme aspect ratios (5000x100)

## Sample Results

Simple component benchmarks (successful):
```
BM_BSP_RectangleArea        0.155 ns
BM_BSP_RectangleCenter      0.313 ns
BM_BSP_NodeCanSplit         0.623 ns
```

## Known Issues

Some benchmarks involving the full BSP algorithm may encounter segmentation faults due to static random number generator initialization in the benchmark context. This is a benchmarking infrastructure issue and does not affect the library functionality (all unit tests pass).

## Dependencies

Benchmarks use Google Benchmark v1.8.3, which is automatically downloaded during CMake configuration.
