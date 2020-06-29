#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <numeric>
#include <math.h>

__global__ void sum(long *output, long chunkSize, int numThreads, long start, long stop)
{
    int index = (threadIdx.x + blockIdx.x * blockDim.x);
    long result = 0;
    long startNum = start + (index * chunkSize);

    for (long i = startNum; i <= stop && i < (startNum + chunkSize); i++) {
        result += i;
    }

    if (result != 0) {
        //printf("startNum: %ld + chunkSize: %ld = result: %ld\n", startNum, startNum + chunkSize, result);
    }

    output[index] = result;

    __syncthreads();

    auto step_size = 1;
    int number_of_threads = numThreads;
    while (number_of_threads > 0)
    {
        if (index < number_of_threads) // still alive?
        {
            const auto first = index * step_size * 2;
            const auto second = first + step_size;
            output[first] += output[second];
        }

        step_size <<= 1;
        number_of_threads >>= 1;
        __syncthreads();
    }
}

int main(int argc, char const *argv[])
{
    long start = 1;
    long stop = 60000;
  
    if (argc == 3)
    {
      start = atol(argv[1]);
      stop = atol(argv[2]);
    }

    int blockSize;      // The launch configurator returned block size 
    int minGridSize;    // The minimum grid size needed to achieve the maximum occupancy for a full device launch 

    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, sum, 0, 0);

    int numThreads = minGridSize * blockSize;
    long chunkSize = ceil((stop - start) / (double)numThreads);

    if (chunkSize < 1) {
        chunkSize = 1;
    }

    const long size = numThreads * sizeof(long);

    long *d;

    cudaMalloc(&d, size);
    cudaMemset(d, 0, size);

    sum<<<minGridSize, blockSize>>>(d, chunkSize, numThreads, start, stop);

    long result;
    cudaMemcpy(&result, d, sizeof(long), cudaMemcpyDeviceToHost);

    std::cout << "Sum is " << result << std::endl;

    cudaFree(d);

    return 0;
}