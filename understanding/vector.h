#include <string>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <random>
#include <source_location>

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>


using std::string;

template <typename T>
class vector
{

constexpr static size_t _DEFAULT_CAP = 7;

void _reallocate(size_t cap_new) {
    T* tmp = new T[cap_new];
    std::cout << "Obtained " << tmp << std::endl;

    for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(m_[i]);
    }

    std::cout << "Freeing " << m_ << std::endl;
    delete[](m_);

    m_ = tmp;
    cap_ = cap_new;

    std::cout << "Reallocated to capacity of " << cap_ << std::endl;
}

public:
~vector()
{
    delete[](m_);
    // std::cout << std::source_location::current().function_name() << "Freed: " << m_ << std::endl;
}

// ctrs
vector()
    : cap_(_DEFAULT_CAP)
    , size_(0)
    , m_(new T[_DEFAULT_CAP])
{
    // std::cout << std::source_location::current().function_name() << "Obtained " << m_ << std::endl;
}

explicit vector(size_t count, const T& value = T())
    : cap_(count)
    , size_(count)
    , m_(new T[count])
{
    for (size_t i = 0; i < size_; i++) {
        m_[i] = value;
    }
}

vector(std::initializer_list<T> l)
    : vector()
{
    for(const T& t: l) {
        this->push_back(t);
    }

}

void push_back(const T& value) {
    m_[size_] = value;
    ++size_;
    // std::cout << "Pushed back: " << value << std::endl;

    if (size_ == cap_) {
        _reallocate(2 * cap_);
    }
}

vector& operator=(std::initializer_list<T> l) {
    delete[](m_);
    cap_ = _DEFAULT_CAP;
    size_ = 0;
    for(const T& t: l) {
        this->push_back(t);
    }

    return *this;
}

T& operator[](size_t i) {
    std::cout << std::source_location::current().function_name() <<": " << i << std::endl;
    const vector<T>& const_ref = *this;
    const T& ret = const_ref[i];
    return const_cast<T&>(ret);
}

const T& operator[](size_t i) const {
    std::cout << std::source_location::current().function_name() <<": " << i << std::endl;
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
    size_t cap_;
    size_t size_;
    T* m_;

};

// int main()
// {
//     // vector<string> vec;
//     // vec.push_back("abc");
//     // vec.push_back("abc2");

//     // const vector<string> vec(3, "abc");
//     // const vector<string> vec {"abc", "abc2"};
//     const vector<string> vec = {"abc", "abc2"};

//     for (size_t i = 0; i < vec.size(); ++i) {
//         const string& curr = vec[i];
//         std::cout << curr << " ";
//     }

//     std::cout << std::endl;
// }

