## Task 3.1: Parallel Sum With CUDA

Implement a GPU-based program that sums up a range of numbers in parallel using CUDA. The general algorithmic problem is called _parallel reduction_. The use of any CUDA-based libraries is prohibited. The goal is to minimize the execution time required to compute the parallel sum compared to the CPU-based version you have implemented in Assignment 1.

### Input

Your executable has to be named parsum and accept two parameters: The start index and the end index (64bit numbers) of the range to compute. For example, the command line call

    ./parsum 1 10000000000
    
has to result in a parallel summation of the numbers 1,2,...,10.000.000.000. 

### Output

Your program has to produce a single line on the standard output, that contains only the computed sum.

### Validation

The solution is considered correct if a true parallelized computation takes place (no Gauss please), and if the application produces correct results for all inputs. We will evaluate your solution with different summation ranges. 
