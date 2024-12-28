#include <chrono>
#include <thread>
#include <mutex>
#include <cstdint>

#include <spdlog/spdlog.h>

using namespace std;

static constexpr int64_t MAX = 999'999;

mutex m;
// volatile int64_t n = 0;
// volatile int64_t n = 0;

atomic<int> n;

// void worker()
// {
//     spdlog::info("Start");

//     auto tp = std::chrono::high_resolution_clock::now();
//     for (int64_t i = 0; i < MAX; ++i) {
//         std::lock_guard<std::mutex> lg(m);
//         ++n;
//     }

//     auto tp2 = std::chrono::high_resolution_clock::now();
//     auto dur = std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp);
//     spdlog::info("End. Took {} microseconds", dur.count());

// }


void worker()
{
    spdlog::info("Start");

    auto tp = std::chrono::high_resolution_clock::now();
    for (int64_t i = 0; i < MAX; ++i) {
        // n.fetch_add(1, memory_order_acquire);
        n.fetch_add(1, memory_order_relaxed);
    }

    auto tp2 = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp);
    spdlog::info("End. Took {} microseconds", dur.count());

}

int main()
{

    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S %F %z][%P %t] %v");

    spdlog::info("Is n is_lock_free? {}", n.is_lock_free());

    thread th1(worker);
    thread th2(worker);

    th1.join();
    th2.join();

    int64_t result = n.load();
    spdlog::info("MAX: {}", MAX);
    spdlog::info("result: {}", result);
    spdlog::info("ratio: {}", static_cast<double>(result)/ MAX);


    return 0;
}