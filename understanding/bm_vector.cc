#include "vector.h"

#include <benchmark/benchmark.h>

using std::string;


static void BM_random_vector(benchmark::State& state) {
  // Perform setup here
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist_size_t(
            std::numeric_limits<size_t>::min(),
            std::numeric_limits<size_t>::max());

    // std::vector<size_t> vec(1024*1024*1024, 0);
    // std::fill(vec.begin(), vec.end(), dist_size_t(rng));

    for (auto _ : state) {
        vector<string> vec;
        for (size_t i = 0; i < 1024*1024; ++i) {
            vec.push_back(std::to_string(dist_size_t(rng)));
        }

    }
}

BENCHMARK(BM_random_vector);
BENCHMARK_MAIN();