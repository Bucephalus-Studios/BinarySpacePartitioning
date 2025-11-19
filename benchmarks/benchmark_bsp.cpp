#include <benchmark/benchmark.h>
#include "benchmark_helpers.hpp"
#include "../BinarySpacePartitioning.hpp"

/**
 * Benchmark: Basic BSP tree creation with small space
 */
static void BSP_SmallSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(100, 100, 10, 4);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_SmallSpace);

/**
 * Benchmark: BSP tree creation with medium space
 */
static void BSP_MediumSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(500, 500, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_MediumSpace);

/**
 * Benchmark: BSP tree creation with large space
 */
static void BSP_LargeSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 20, 32);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_LargeSpace);

/**
 * Benchmark: BSP tree creation with very large space
 */
static void BSP_VeryLargeSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(2000, 2000, 20, 64);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_VeryLargeSpace);

/**
 * Benchmark: Varying partition counts
 */
static void BSP_VaryingPartitions(benchmark::State& state) {
    int partitions = state.range(0);
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, partitions);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_VaryingPartitions)->Range(4, 128);

/**
 * Benchmark: Varying minimum split sizes
 */
static void BSP_VaryingMinSize(benchmark::State& state) {
    int minSize = state.range(0);
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, minSize, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_VaryingMinSize)->Range(5, 100);

/**
 * Benchmark: BSP with geometric centers (default)
 */
static void BSP_GeometricCenters(benchmark::State& state) {
    BSP_Settings settings;
    settings.randomPartitionCenters = false;

    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32, settings);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_GeometricCenters);

/**
 * Benchmark: BSP with random centers
 */
static void BSP_RandomCenters(benchmark::State& state) {
    BSP_Settings settings;
    settings.randomPartitionCenters = true;

    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32, settings);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_RandomCenters);

/**
 * Benchmark: Node creation
 */
static void BSP_NodeCreation(benchmark::State& state) {
    BSP_Tree tree;
    BSP_Rectangle rect(0, 0, 100, 100);

    for (auto _ : state) {
        auto node = tree.createNode(rect);
        benchmark::DoNotOptimize(node);
    }
}
BENCHMARK(BSP_NodeCreation);

/**
 * Benchmark: Node splitting
 */
static void BSP_NodeSplitting(benchmark::State& state) {
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
BENCHMARK(BSP_NodeSplitting);

/**
 * Benchmark: Getting leaf nodes from tree
 */
static void BSP_GetLeafNodes(benchmark::State& state) {
    // Setup: Create a tree once
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 32);

    for (auto _ : state) {
        auto leafNodes = tree.getLeafNodes();
        benchmark::DoNotOptimize(leafNodes);
    }
}
BENCHMARK(BSP_GetLeafNodes);

/**
 * Benchmark: Getting random leaf nodes
 */
static void BSP_GetRandomLeafNodes(benchmark::State& state) {
    // Setup: Create a tree once
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 64);
    int count = state.range(0);

    for (auto _ : state) {
        auto randomLeaves = tree.getRandomLeafNodes(count);
        benchmark::DoNotOptimize(randomLeaves);
    }
}
BENCHMARK(BSP_GetRandomLeafNodes)->Range(1, 32);

/**
 * Benchmark: Rectangle area calculation
 */
static void BSP_RectangleArea(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);

    for (auto _ : state) {
        int area = rect.getArea();
        benchmark::DoNotOptimize(area);
    }
}
BENCHMARK(BSP_RectangleArea);

/**
 * Benchmark: Rectangle center retrieval
 */
static void BSP_RectangleCenter(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);

    for (auto _ : state) {
        auto center = rect.getCenter();
        benchmark::DoNotOptimize(center);
    }
}
BENCHMARK(BSP_RectangleCenter);

/**
 * Benchmark: Node canSplit check
 */
static void BSP_NodeCanSplit(benchmark::State& state) {
    BSP_Rectangle rect(0, 0, 1000, 1000);
    BSP_Node node(rect, 0);

    for (auto _ : state) {
        bool result = node.canSplit(10);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BSP_NodeCanSplit);

/**
 * Benchmark: Tree node count
 */
static void BSP_TreeNodeCount(benchmark::State& state) {
    auto tree = BinarySpacePartitioning::runBSP(1000, 1000, 10, 64);

    for (auto _ : state) {
        int count = tree.getNodeCount();
        benchmark::DoNotOptimize(count);
    }
}
BENCHMARK(BSP_TreeNodeCount);

/**
 * Benchmark: Non-square spaces
 */
static void BSP_NonSquareSpace(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(2000, 500, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_NonSquareSpace);

/**
 * Benchmark: Extreme aspect ratio
 */
static void BSP_ExtremeAspectRatio(benchmark::State& state) {
    for (auto _ : state) {
        auto tree = BinarySpacePartitioning::runBSP(5000, 100, 10, 16);
        benchmark::DoNotOptimize(tree);
    }
}
BENCHMARK(BSP_ExtremeAspectRatio);

BENCHMARK_MAIN();
