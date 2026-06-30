cmake_minimum_required(VERSION 3.16)
project(BinarySpacePartitioning LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(stevensMathLib
    GIT_REPOSITORY https://github.com/Bucephalus-Studios/stevensMathLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(stevensMathLib)

add_library(BinarySpacePartitioning INTERFACE)
add_library(BinarySpacePartitioning::BinarySpacePartitioning ALIAS BinarySpacePartitioning)

target_include_directories(BinarySpacePartitioning INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(BinarySpacePartitioning INTERFACE cxx_std_17)
target_link_libraries(BinarySpacePartitioning INTERFACE stevensMathLib)

option(BINARYSPACEPARTITIONING_BUILD_TESTS "Build tests" OFF)
option(BINARYSPACEPARTITIONING_BUILD_BENCHMARKS "Build benchmarks" OFF)

if(BINARYSPACEPARTITIONING_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BINARYSPACEPARTITIONING_BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(benchmark)
    add_subdirectory(benchmarks)
endif()
