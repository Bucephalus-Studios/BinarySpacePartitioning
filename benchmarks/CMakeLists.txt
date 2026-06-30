add_executable(bsp_benchmarks
    benchmark_bsp.cpp
)

target_link_libraries(bsp_benchmarks
    PRIVATE
        BinarySpacePartitioning
        benchmark::benchmark
)
