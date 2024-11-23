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



int main()
{

    const auto lam = [](int x) -> int {
        std::cout << std::source_location::current().function_name() << std::endl;
        return x * 2;
    };

    std::cout << lam(5) << std::endl;
    std::cout << lam(10) << std::endl;
    return 0;
}