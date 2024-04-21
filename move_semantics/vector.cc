#include <string>
#include <iostream>

using std::string;

template <typename T>
class vector
{

constexpr static size_t _DEFAULT_CAP = 7;

void _reallocate(size_t cap_new) {
    T* tmp = reinterpret_cast<T*>(malloc(sizeof(T) * cap_new));
    for (size_t i = 0; i < size_; ++i) {
        tmp[i] = m_[i];
    }

    free(m_);
    m_ = tmp;
    cap_ = cap_new;

    std::cout << "Reallocated to capacity of " << cap_ << std::endl;
}
public:
    vector()
        : m_(reinterpret_cast<T*>(malloc(sizeof(T) * _DEFAULT_CAP)))
        , size_(0)
        , cap_(_DEFAULT_CAP)
    {
    }

    explicit vector(size_t count, const T& value = T())
        : size_(count)
        , cap_(count)
    {
        m_ = reinterpret_cast<T*>(malloc(sizeof(T) * count));
        for (size_t i = 0; i < size_; i++) {
            m_[i] = value;
        }
    }

    void push_back(const T& value) {
        if (size_ == cap_) {
            _reallocate(2 * cap_);
        }

        m_[size_] = value;
        size_++;
    }

    T& operator [] (size_t i) {
        return m_[i];
    }

    const T& operator [] (size_t i) const {
        return m_[i];
    }

    T& front() {
        return m_[0];
    }

    T& back() {
        return m_[size_ - 1];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return cap_;
    }

private:
    T* m_;
    size_t size_;
    size_t cap_;

};

int main()
{
    vector<string> vec;
    vec.push_back("abc");
    vec.push_back("abc2");
    vec.push_back("abc3");
    vec.push_back("abc4");
    vec.push_back("abc5");
    vec.push_back("abc6");
    vec.push_back("abc7");
    vec.push_back("abc8");

    // for (const auto& v: vec) {
    //     std::cout << v << " ";
    // }

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }

    std::cout << std::endl;

    return 0;
}

// #include "gtest/gtest.h"

// TEST( ExampleTest, First ) {
//   EXPECT_TRUE(  true );
// }

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }