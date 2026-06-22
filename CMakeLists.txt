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
