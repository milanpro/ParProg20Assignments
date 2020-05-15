## Task 1.4

Write and evaluate a program `mcpi`  approximates $`\pi`$ using the Monte-Carlo-Method using C++ `std::thread`.

Use multiple threads to generate a number of random points $`(x,y) \in [0,1] \times [0,1]`$.
For each point determine whether it falls within the quarter unit circle centered at $`(0,0)`$, i.e. whether it satisfies $`x^2 + y^2 \le 1`$. Approximate $`\pi`$ at `double` precision with:

$`\pi = 4 \frac{\text{points in circle}}{\text{number of points}}`$

For comparable results use one [`std::mt19937`](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine) pseudo-number generator per thread, seeded with the thread id (zero-based index). First generate the x value, than the y value for each point.

    std::mt19937 generator(thread_id);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
	for (int i = 0; i < npoints; i++)
		std::cout
			<< "x: " << dist(generator) << ", " 
			<< "y: " << dist(generator) << std::endl;


If the number if points is not divisible by the number of threads, the last thread should do the remainder points.


Implement three strategies, to determine how many points are within the circle:
1. All threads count their circle points independently in local variables. Add final aggregation step(s).
1. All threads count their circle points using one global variable. Use synchronization mechanisms to protect this shared resource.
1. All threads count their circle points using one global variable, with atomic operations from `std::atomic`.

### Input

The number of tested points (64 bit) and the number of started threads are passed as command line arguments. The third argument is the chosen strategy.

You should include all strategies within one binary.

    
### Output

Your program prints a single line on the standard output containing the approximation of $`\pi`$ with **15 significant digits**.

Example call for 100000 points, 1 thread, and strategy 2:

    ./mcpi 100000 1 2
    3.145000000000000

### Experiment

Compare all strategies experiementally by computing the Karp-Flatt-Metric for the following number of threads: 1, 2, 3, 4, 6, 8, 16, 32, 64, each for a problem size of 10,000,000, 100,000,000, and 1,000,000,000 points. 

Measure the runtime using `/usr/bin/time`. Repeat every experiment four times. Produce a `measurement.csv` of the following format:

    strategy,nthreads,npoints,runtime
    1,1,10000000,...
    1,1,10000000,...
    1,1,10000000,...
    1,1,10000000,...
    1,1,10000000000,...
    ...
    1,2,10000000,...
    ...
    
Calculate the Karp-Flatt-Metric for each strategy and workload size, as a function of the thread count.
In a file called `metrics.csv`, give the serial fractions (as numbers in range 0 to 1) for each series:

    strategy,npoints,q2,q3,q4,q6,q8,q12,q16,q32,q64
    1,10000000,...,...,...,...,...,...,...,...,...
    ...

### Discussion

Submit a file `discussion.txt`, where you answer the following questions:

* What can you observe from your Q-series?
* Did you find parallelization overhead? What might cause/prevent it?
* Which external factors can influence your measurements?