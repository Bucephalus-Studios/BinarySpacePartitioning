cmake_minimum_required(VERSION 3.14)
project(BinarySpacePartitioning_Tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module to download GoogleTest
include(FetchContent)

# Download and configure GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add test executables
add_executable(bsp_tests
    test_bsp_rectangle.cpp
    test_bsp_node.cpp
    test_bsp_tree.cpp
    test_bsp_algorithm.cpp
)

# Link against gtest and gtest_main
target_link_libraries(bsp_tests
    gtest
    gtest_main
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(bsp_tests)
