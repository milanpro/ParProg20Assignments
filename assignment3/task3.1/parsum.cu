#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <numeric>

__global__ void sum(int *output, int chunkSize, int numThreads, int start, int stop)
{
    int index = (threadIdx.x + blockIdx.x * blockDim.x);
    int result = 0;
    int startNum = start + (index * chunkSize);
    for (int i = startNum; i < stop && i < (startNum + chunkSize); i++) {
        result += i;
    }

    output[index] = result

    auto step_size = 1;
    int number_of_threads = numThreads;
    while (number_of_threads > 0)
    {
        if (index < number_of_threads) // still alive?
        {
            const auto first = index * step_size * 2;
            const auto second = first + step_size;
            input[first] += input[second];
        }

        step_size <<= 1;
        number_of_threads >>= 1;
    }
}

int main(int argc, char const *argv[])
{
    long start = 1;
    long stop = 6074001000;
  
    if (argc == 3)
    {
      start = atol(argv[1]);
      stop = atol(argv[2]);
    }

    int blockSize;      // The launch configurator returned block size 
    int minGridSize;    // The minimum grid size needed to achieve the maximum occupancy for a full device launch 

    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, sum, 0, 0); 

    int numThreads = minGridSize * blockSize;
    int chunkSize = (stop - start) / numThreads;

    const int size = numThreads * sizeof(int);

    int *d;

    cudaMalloc(&d, size);
    cudaMemset(d, 0, size);

    sum<<<1, count / 2>>>(d, chunkSize, numThreads, start, stop);

    int result;
    cudaMemcpy(&result, d, sizeof(int), cudaMemcpyDeviceToHost);

    std::cout << "Sum is " << result << std::endl;

    cudaFree(d);

    return 0;
}