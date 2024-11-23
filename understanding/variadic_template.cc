
#include <iostream>
#include <string>
#include <utility>

template <typename T0, typename ... Ts>
void print(T0&& t0, Ts&&... ts)
{
    std::cout << std::forward<T0>(t0) << "\n";

    if constexpr(sizeof...(ts)) {
        print(std::forward<Ts>(ts)...);
    }
}

template <typename ... Ts>
void print2(Ts... ts)
{
    (std::cout << ... << std::forward<Ts>(ts)) << "\n";
    // << std::endl;
}

template <typename ... Ts>
void print3(Ts... ts)
{
    (std::cout << ... << std::forward<Ts>(ts)) << "\n";
    // << std::endl;
}

int main ()
{
    // print("123");
    // print("123", 456);
    print3("123", 456);

}