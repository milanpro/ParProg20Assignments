#include <thread>
#include <cmath>

int fib(int n)
{
    double phi = (1 + sqrt(5)) / 2;
    return round(pow(phi, n) / sqrt(5));
}

#define ITERATIONS 100000000000ULL

void threaded_fib()
{
    for (;;)
    {
        unsigned long long v = 0;
        for (unsigned long long i = 0; i < ITERATIONS; ++i)
        {
            v += fib(i);
        }
    }
}

void benchmark()
{
    std::thread kevin(threaded_fib);
    std::thread bob(threaded_fib);

    kevin.join();
    bob.join();
}

int main(int argc, char const *argv[])
{
    benchmark();
    return 0;
}