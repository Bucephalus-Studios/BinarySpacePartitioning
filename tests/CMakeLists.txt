add_executable(bsp_tests
    test_bsp_rectangle.cpp
    test_bsp_node.cpp
    test_bsp_tree.cpp
    test_bsp_algorithm.cpp
)

target_link_libraries(bsp_tests
    PRIVATE
        BinarySpacePartitioning
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bsp_tests)
