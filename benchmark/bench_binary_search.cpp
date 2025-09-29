#include <benchmark/benchmark.h>

#include "../src/include/binary_search.hpp"
#include <vector>

static void BM_BinarySearch(benchmark::State& state) {
    int size = state.range(0);
    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = i * 2; // Even numbers
    }
    // Pick a guaranteed-present target to avoid exceptions:
    int target = data[size / 2];

    for (auto _ : state) {
        BinarySearch::search(data, target);
        benchmark::ClobberMemory(); // prevent optimizer from removing the call
    }
}

BENCHMARK(BM_BinarySearch)->Arg(1<<10)->Arg(1<<20);

BENCHMARK_MAIN();