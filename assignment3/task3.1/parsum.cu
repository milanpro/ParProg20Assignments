#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <numeric>
#include <cmath>

typedef unsigned long long int number_type;

__global__ void sum(number_type *input)
{
    const int tid = blockIdx.x * blockDim.x + threadIdx.x;

    number_type start = input[tid * 2];
    number_type stop = input[tid * 2 + 1];

    // Reuse the input array to save the result
    input[tid * 2] = 0;
    input[tid * 2 + 1] = 0;

    for (number_type i = start; i <= stop; i++)
    {
        number_type old = input[tid * 2 + 1];
        input[tid * 2 + 1] += i;

        // Detect overflow
        if (old > input[tid * 2 + 1])
        {
            input[tid * 2]++;
        }
    }
}

int main(int argc, char const *argv[])
{
    number_type start = 1;
    number_type stop = 1000;

    if (argc == 3)
    {
        start = atol(argv[1]);
        stop = atol(argv[2]);
    }


    const number_type range_per_thread = min(stop - start, 100ull);
    const number_type threads_per_block = 256;

    number_type num_threads = (stop - start + 1) / range_per_thread;
    number_type num_blocks = std::ceil((double) num_threads / (double) threads_per_block);

    number_type size = sizeof(number_type) * num_blocks * threads_per_block * 2;
    number_type *ranges = (number_type *)malloc(size);
    number_type current = start;


    for (int i = 0; i < num_threads; i++)
    {
        unsigned long long range_per_thread = (double)(stop - current) / (double)(num_threads - i);

        unsigned long long end = current + range_per_thread;
        ranges[i * 2] = current;
        ranges[i * 2 + 1] = end;

        current = end + 1;
    }

    number_type *ranges_gpu;
    cudaMalloc(&ranges_gpu, size);
    cudaMemcpy(ranges_gpu, ranges, size, cudaMemcpyHostToDevice);

    sum<<<num_blocks, threads_per_block>>>(ranges_gpu);

    /*
    cudaError_t err = cudaGetLastError();
    const char* error = cudaGetErrorString(err);
    printf("%s\n", error);
    */

    cudaMemcpy(ranges, ranges_gpu, size, cudaMemcpyDeviceToHost);


    __uint128_t result = 0;

    for (number_type i = 0; i < num_threads; i++)
    {
        __uint128_t upper = ranges[i * 2];
        __uint128_t lower = ranges[i * 2 + 1];

        upper <<= 64;

        result += upper;
        result += lower;
    }

    char arr[39];
    char basechars[] = "0123456789";
    int length = 0;
    while (result != 0)
    {
        int radix;
        radix = result % 10;
        result = result / 10;
        arr[length++] = basechars[radix];
    }
    if (length == 0)
    {
        std::cout << 0 << std::endl;
    }
    else
    {
        while (length--)
            std::cout << arr[length];

        std::cout << std::endl;
    }

    cudaFree(ranges_gpu);

    return 0;
}