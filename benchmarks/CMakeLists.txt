cmake_minimum_required(VERSION 3.14)
project(BinarySpacePartitioning_Benchmarks)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module to download Google Benchmark
include(FetchContent)

# Download and configure Google Benchmark
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)

# Don't build benchmark tests
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(benchmark)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add benchmark executables
add_executable(bsp_benchmarks
    benchmark_bsp.cpp
)

# Link against benchmark library
target_link_libraries(bsp_benchmarks
    benchmark::benchmark
)
