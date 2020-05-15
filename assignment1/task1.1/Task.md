## Task 1.1

Implement a program that sums up a range of numbers in parallel using PThread. The general algorithmic problem is called *parallel reduction*.


### Input

Your executable has to be named `parsum` and accept three parameters: The number of threads to use, the start index and the end index (64bit numbers) of the range to compute. For example, the command line

    ./parsum 30 1 10000000000

has to result in a parallel summation of the numbers 1,2,...,10.000.000.000, based on 30 threads running in parallel. 


### Output

Your program has to produce a single line on the standard output, that contains only the computed sum.
Validation

Your solution is considered correct if a true parallelized computation takes place (no Gauss please), if the solution scales based on the number of threads, and if the application produces correct results for all inputs. We will evaluate your solution with different thread counts / summation ranges. 
