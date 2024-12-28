#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <span>

#include <benchmark/benchmark.h>


static void BM_ref(benchmark::State& state) {
    std::vector<std::string> vec;

    for (size_t i = 0; i < 1000; ++i) {
        vec.emplace_back(std::to_string(i));
    }

    size_t size_sum = 0;
    for (auto _ : state) {
        for (const auto& str: vec) {
            size_sum += str.size();
        }
    }

    std::cout << "size_sum: " << size_sum << std::endl;
}

static void BM_string_view(benchmark::State& state) {
    std::vector<std::string> vec;

    for (size_t i = 0; i < 1000; ++i) {
        vec.emplace_back(std::to_string(i));
    }

    size_t size_sum = 0;
    for (auto _ : state) {
        for (const std::string_view& view: vec) {
            size_sum += view.size();

        }

    }

    std::cout << "size_sum: " << size_sum << std::endl;

}

static void BM_span(benchmark::State& state) {
    std::vector<std::string> vec;

    for (size_t i = 0; i < 1000; ++i) {
        vec.emplace_back(std::to_string(i));
    }

    std::span s(vec);
    size_t size_sum = 0;
    for (auto _ : state) {
        for (const auto& iter: s) {
            size_sum += iter.size();
        }

    }

    std::cout << "size_sum: " << size_sum << std::endl;
}

BENCHMARK(BM_ref);
BENCHMARK(BM_string_view);
BENCHMARK(BM_span);
BENCHMARK_MAIN();