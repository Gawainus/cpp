#include <thread>
#include <iostream>

struct Worker
{
void work()
{
    std::cout << "Worker" << std::endl;
}
    // int n = 0;

};

int main()
{

    Worker w;
    std::thread th(&Worker::work, &w);

    th.join();

}

// int main()
// {
//     int n = 0;
//     Worker w;
//     std::thread t4(&Worker::work, &w); // t5 runs foo::bar() on object f
//     t4.join();
//     std::cout << "Final value of n is " << n << '\n';
// }

// class foo
// {
// public:
//     void bar()
//     {
//         for (int i = 0; i < 5; ++i)
//         {
//             std::cout << "Thread 3 executing\n";
//             ++n;
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         }
//     }
//     int n = 0;
// };


// int main()
// {
//     int n = 0;
//     Worker w;
//     foo f;
//     std::thread t4(&Worker::work, &w); // t5 runs foo::bar() on object f
//     std::thread t5(&foo::bar, &f); // t5 runs foo::bar() on object f
//     t4.join();
//     t5.join();
//     std::cout << "Final value of n is " << n << '\n';
//     std::cout << "Final value of f.n (foo::n) is " << f.n << '\n';
// }