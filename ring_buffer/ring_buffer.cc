#include <cassert>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>

constexpr size_t CAPACITY = std::pow(1024, 2);
std::array<size_t, CAPACITY> arr_;

std::atomic<size_t> idx_head_ = 0;
std::atomic<size_t> idx_tail_ = 0;
std::atomic<size_t> buffer_size_ = 0;
std::atomic<size_t> num_failures_ = 0;


template<typename T, size_t SIZE>
class RingBuffer
{

public:

RingBuffer()
{
    std::cout << "Ctr: idx_head_: :" << idx_head_.load()
        <<  " idx_tail_: " << idx_tail_.load()  << std::endl;
}

RingBuffer(RingBuffer&& rb)
{

}


const T& front() {
    return arr_[idx_head_.load(std::memory_order_acquire)];
}


const T& tail() {
    return arr_[idx_tail_.load(std::memory_order_acquire)];
}

void push_back(T&& value) {
    size_t reserved_idx = idx_tail_.load(std::memory_order_acquire);
    size_t tail_new = (reserved_idx + 1) % (CAPACITY);

    while(!idx_tail_.compare_exchange_strong(reserved_idx, tail_new)) {
        // std::cout << "Revervation failed expected tail: " << reserved_idx << " tail_new: " << tail_new << std::endl;
        tail_new = reserved_idx + 1 % (CAPACITY - 1);
        num_failures_.fetch_add(1, std::memory_order_acquire);

    }

    arr_[reserved_idx] = value;
    buffer_size_.fetch_add(1, std::memory_order_acquire);
    // size_t head = idx_head_.load(std::memory_order_acquire);
    // size_t tail = idx_tail_.load(std::memory_order_acquire);
    // size_t buffer_size = buffer_size_.fetch_add(1, std::memory_order_acquire);
    // std::cout << "reserved: " << reserved_idx
    //     << " buffer_size: " << buffer_size
    //     << " head: " << head << " tail: " << tail << std::endl;
    // std::cout << "Revervation exceeded expected tail: " << expected << " reserved_idx: " << reserved_idx << std::endl;
}

// void push_back(const T& value) {
//     this->push_back(stdvalue);
// }

void pop() {
    size_t size = buffer_size_.load(std::memory_order_acquire);
    if (size == 0) {
        return;
    }


    size_t idx_tail = idx_tail_.load(std::memory_order_acquire);
    size_t idx_head = idx_head_.load(std::memory_order_acquire);



}

private:


};

template<typename T, size_t SIZE>
struct Producer
{

Producer(RingBuffer<size_t, CAPACITY>* rb): rb_(rb) { }

void produce(size_t start_v, size_t end_v)
{
    for (size_t i = start_v; i <= end_v; ++i) {
        rb_->push_back(std::move(i));
    }
}

RingBuffer<size_t, CAPACITY>* rb_;
};


int main()
{
    RingBuffer<size_t, CAPACITY> rb;
    size_t num_th = 4;
    std::vector<std::thread> th_pool;
    size_t num_ticks =  CAPACITY / num_th;
    std::cout << "num_ticks per thread: " << num_ticks << std::endl;
    size_t sum = 0;
    for (size_t i = 0; i < num_th; ++i) {
        size_t start_v = i * num_ticks;
        size_t end_v = (i + 1) * num_ticks - 1;
        Producer<size_t, CAPACITY> p(&rb);
        std::thread th(&Producer<size_t, CAPACITY>::produce,&p,
            start_v, end_v);

        sum += (end_v * (end_v + 1)) / 2 - sum;
        std::cout << "thread [" << th.get_id() << "] is working from "
            << start_v << " to " << end_v << std::endl;


        th_pool.emplace_back(std::move(th));

    }

    for (auto& th: th_pool) {
        th.join();
    }

    size_t head = idx_head_.load(std::memory_order_acquire);
    size_t buffer_size = buffer_size_.load(std::memory_order_acquire);
    size_t sum_arr = 0;
    for (size_t i = head; i < buffer_size; ++i) {
        sum_arr += arr_[i % CAPACITY];
    }

    std::cout << "Sum of buffer: " << sum_arr << " sum: " << sum << std::endl;
    if (sum_arr != sum) {
        throw std::runtime_error("sum_arr and sum do not equal");
    }


    size_t num_failures = num_failures_.load(std::memory_order_acquire);
    std::cout << "CAPACITY: " << CAPACITY
        << "\nhead: " << head
        << "\nbuffer_size: " << buffer_size
        << "\nnum_failures: " << num_failures
        << "\nfailure to size ratio: "
        << static_cast<double>(num_failures) / CAPACITY
        << std::endl;
}

// int main()
// {
//     RingBuffer<size_t, CAPACITY> rb;
//     size_t num_th = 4;
//     std::vector<std::thread> th_pool;
//     size_t num_ticks =  CAPACITY / num_th;
//     std::cout << "num_ticks per thread: " << num_ticks << std::endl;
//     size_t sum = 0;
//     for (size_t i = 0; i < num_th; ++i) {
//         size_t start_v = i * num_ticks;
//         size_t end_v = (i + 1) * num_ticks - 1;
//         Producer<size_t, CAPACITY> p(&rb);
//         std::thread th(&Producer<size_t, CAPACITY>::produce,&p,
//             start_v, end_v);

//         sum += (end_v * (end_v + 1)) / 2 - sum;
//         std::cout << "thread [" << th.get_id() << "] is working from "
//             << start_v << " to " << end_v << std::endl;


//         th_pool.emplace_back(std::move(th));

//     }

//     for (auto& th: th_pool) {
//         th.join();
//     }

//     size_t head = idx_head_.load(std::memory_order_acquire);
//     size_t buffer_size = buffer_size_.load(std::memory_order_acquire);
//     size_t sum_arr = 0;
//     for (size_t i = head; i < buffer_size; ++i) {
//         sum_arr += arr_[i % CAPACITY];
//     }

//     std::cout << "Sum of buffer: " << sum_arr << " sum: " << sum << std::endl;
//     if (sum_arr != sum) {
//         throw std::runtime_error("sum_arr and sum do not equal");
//     }


//     size_t num_failures = num_failures_.load(std::memory_order_acquire);
//     std::cout << "CAPACITY: " << CAPACITY
//         << "\nhead: " << head
//         << "\nbuffer_size: " << buffer_size
//         << "\nnum_failures: " << num_failures
//         << "\nfailure to size ratio: "
//         << static_cast<double>(num_failures) / CAPACITY
//         << std::endl;
// }


