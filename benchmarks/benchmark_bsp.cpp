#include <benchmark/benchmark.h>
#include "benchmark_helpers.hpp"
#include "../BinarySpacePartitioning.hpp"

/**
 * Benchmark: Basic BSP tree creation with small space
 */
static void BM_BSP_SmallSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(100, 100, 10, 4);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_SmallSpace);

/**
 * Benchmark: BSP tree creation with medium space
 */
static void BM_BSP_MediumSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(500, 500, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_MediumSpace);

/**
 * Benchmark: BSP tree creation with large space
 */
static void BM_BSP_LargeSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 20, 32);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_LargeSpace);

/**
 * Benchmark: BSP tree creation with very large space
 */
static void BM_BSP_VeryLargeSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(2000, 2000, 20, 64);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_VeryLargeSpace);

/**
 * Benchmark: Varying partition counts
 */
static void BM_BSP_VaryingPartitions(benchmark::State& state) {
    int partitions = state.range(0);
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, partitions);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_VaryingPartitions)->Range(4, 128);

/**
 * Benchmark: Varying minimum split sizes
 */
static void BM_BSP_VaryingMinSize(benchmark::State& state) {
    int minSize = state.range(0);
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, minSize, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_VaryingMinSize)->Range(5, 100);

/**
 * Benchmark: BSP with geometric centers (default)
 */
static void BM_BSP_GeometricCenters(benchmark::State& state) {
    BSP_Settings settings;
    settings.randomPartitionCenters = false;

    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32, settings);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_GeometricCenters);

/**
 * Benchmark: BSP with random centers
 */
static void BM_BSP_RandomCenters(benchmark::State& state) {
    BSP_Settings settings;
    settings.randomPartitionCenters = true;

    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32, settings);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_RandomCenters);

/**
 * Benchmark: Node creation
 */
static void BM_BSP_NodeCreation(benchmark::State& state) {
    BSP_Tree tree;
    BSP_Rectangle rect(0, 0, 100, 100);

    for (auto _ : state) {
        auto node = tree.createNode(rect);
        benchmark::DoNotOptimize(node);
    }
}
BENCHMARK(BM_BSP_NodeCreation);

/**
 * Benchmark: Node splitting
 */
static void BM_BSP_NodeSplitting(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        BSP_Tree tree;
        BSP_Rectangle rect(0, 0, 1000, 1000);
        auto node = tree.createNode(rect);
        auto nodeCreator = [&tree](const BSP_Rectangle& r) {
            return tree.createNode(r);
        };
        state.ResumeTiming();

        node->split(BSP_SplitDirection::Horizontal, 10, nodeCreator);
        benchmark::DoNotOptimize(node);
    }
}
BENCHMARK(BM_BSP_NodeSplitting);

/**
 * Benchmark: Getting leaf nodes from tree
 */
static void BM_BSP_GetLeafNodes(benchmark::State& state) {
    // Setup: Create a tree once
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32);

    for (auto _ : state) {
        auto leafNodes = tree.getLeafNodes();
        benchmark::DoNotOptimize(leafNodes);
    }
}
BENCHMARK(BM_BSP_GetLeafNodes);

/**
 * Benchmark: Getting random leaf nodes
 */
static void BM_BSP_GetRandomLeafNodes(benchmark::State& state) {
    // Setup: Create a tree once
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 64);
    int count = state.range(0);

    for (auto _ : state) {
        auto randomLeaves = tree.getRandomLeafNodes(count);
        benchmark::DoNotOptimize(randomLeaves);
    }
}
BENCHMARK(BM_BSP_GetRandomLeafNodes)->Range(1, 32);

/**
 * Benchmark: Rectangle area calculation
 */
static void BM_BSP_RectangleArea(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);

    for (auto _ : state) {
        int area = rect.getArea();
        benchmark::DoNotOptimize(area);
    }
}
BENCHMARK(BM_BSP_RectangleArea);

/**
 * Benchmark: Rectangle center retrieval
 */
static void BM_BSP_RectangleCenter(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);

    for (auto _ : state) {
        auto center = rect.getCenter();
        benchmark::DoNotOptimize(center);
    }
}
BENCHMARK(BM_BSP_RectangleCenter);

/**
 * Benchmark: Node canSplit check
 */
static void BM_BSP_NodeCanSplit(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);
    BSP_Node node(rect, 0);

    for (auto _ : state) {
        bool result = node.canSplit(10);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_BSP_NodeCanSplit);

/**
 * Benchmark: Tree node count
 */
static void BM_BSP_TreeNodeCount(benchmark::State& state) {
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 64);

    for (auto _ : state) {
        int count = tree.getNodeCount();
        benchmark::DoNotOptimize(count);
    }
}
BENCHMARK(BM_BSP_TreeNodeCount);

/**
 * Benchmark: Non-square spaces
 */
static void BM_BSP_NonSquareSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(2000, 500, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_NonSquareSpace);

/**
 * Benchmark: Extreme aspect ratio
 */
static void BM_BSP_ExtremeAspectRatio(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(5000, 100, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BM_BSP_ExtremeAspectRatio);

BENCHMARK_MAIN();
