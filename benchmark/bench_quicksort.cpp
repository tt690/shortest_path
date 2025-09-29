#include <benchmark/benchmark.h>

#include "../src/include/binary_search.hpp"

static void BM_Quicksort(benchmark::State& state) {
    int size = state.range(0);
    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = size - i;
    }

    for (auto _ : state) {
        std::vector<int> data_copy = data;
        BinarySearch::quicksort(data_copy, 0, size - 1);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Quicksort)->Arg(1<<5)->Arg(1<<10);
BENCHMARK_MAIN();