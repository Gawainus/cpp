
#include <algorithm>
#include <memory>
#include <vector>
#include <random>

#include <benchmark/benchmark.h>

using std::vector;

int add(int a, int b)
{
    return a + b;
}

struct Sum
{
    void operator()(int n) { sum += n; }
    int sum{0};
};

template<class T>
T sum(const vector<T>& vec)
{
    Sum s = std::for_each(vec.begin(), vec.end(),  Sum());
    return s.sum;
}

static void BM_random_vector(benchmark::State& state) {
  // Perform setup here
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist_size_t(
            std::numeric_limits<size_t>::min(),
            std::numeric_limits<size_t>::max());

    std::vector<size_t> vec(1024*1024*1024, 0);
    // std::fill(vec.begin(), vec.end(), dist_size_t(rng));

    for (auto _ : state) {
        // This code gets timed
        sum<size_t>(vec);
    }
}

BENCHMARK(BM_random_vector);
BENCHMARK_MAIN();
