#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

#include <benchmark/benchmark.h>


static void BM_map_1(benchmark::State& state) {
    std::unordered_map<int64_t, int64_t> ds;

    const size_t MAX = std::pow(2, 1);
    for (size_t i = 0; i < MAX; ++i) {
        ds[i] = i;
    }

    size_t value_sum = 0;
    for (auto _ : state) {
        value_sum += ds[1];
    }

    std::cout << "value_sum: " << value_sum << std::endl;
}

static void BM_unordered_map_1(benchmark::State& state) {
    std::unordered_map<int64_t, int64_t> ds;

    const size_t MAX = std::pow(2, 1);
    for (size_t i = 0; i < MAX; ++i) {
        ds[i] = i;
    }

    size_t value_sum = 0;
    for (auto _ : state) {
            value_sum += ds[1];
    }

    std::cout << "value_sum: " << value_sum << std::endl;
}


constexpr size_t POW_2_20 = std::pow(2, 30);
static void BM_map_20(benchmark::State& state) {
    std::unordered_map<int64_t, int64_t> ds;

    for (size_t i = 0; i < POW_2_20; ++i) {
        ds[i] = i;
    }

    size_t value_sum = 0;
    for (auto _ : state) {
            value_sum += ds[2];
    }

    std::cout << "value_sum: " << value_sum << std::endl;
}

static void BM_unordered_map_20(benchmark::State& state) {
    std::unordered_map<int64_t, int64_t> ds;

    for (size_t i = 0; i < POW_2_20; ++i) {
        ds[i] = i;
    }

    size_t value_sum = 0;
    for (auto _ : state) {
            value_sum += ds[2];

    }

    std::cout << "value_sum: " << value_sum << std::endl;
}

// BENCHMARK(BM_map_1);
// BENCHMARK(BM_unordered_map_1);
BENCHMARK(BM_map_20);
BENCHMARK(BM_unordered_map_20);
BENCHMARK_MAIN();