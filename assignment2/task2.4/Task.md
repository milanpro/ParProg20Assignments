## Task 2.4: ILP and SMT Characterization

Design and implement microbenchmarks to evaluate the ILP and SMT characteristics of your platform.
Execute your microbenchmarks on a single core and record their average performance using the `perf` tool, excluding potential warm-up phases behavior.

Use C/C++ to implement each microbenchmark. You may use platform-independent optimization flags (e.g. `-O3`, `-Os`), but if you do so, explain which is the intended effect on the behavior.

To gauge the behavior of your code, you can examine the assembly listing produced by your compiler. (i.e. `gcc -S`) Alternatively, you might use this online [*Compiler Explorer*](`https://godbolt.org/`).

The execution characteristics of your microbenchmark should depend exclusively on your code, so avoid external library calls whithin the microbenchmark-proper.

During the initialization phase, external calls can be used freely. Be sure to exclude the initialization phase from your measurements.
It is fine if you use the method we demonstrated in the hands-on sessions, where you signify the end of the initialization phase by printing a message to stdout and run `perf -I <interval>` in periodic mode.

After recording the data for each of the following experiments, discuss your results.

### Experiment A: Warm Up

Consider the following program:

    #define ITERATIONS 100000000000ULL

    void benchmark() {
      for (;;) {
        unsigned long long v = 0;
        for (unsigned long long i = 0; i < ITERATIONS; ++i) {
          v *= v * i;
        }    
      }
    }

    int main() {
      // initialize();
      benchmark();
      return 0;
    }

Execute the program in three versions:
* as it is
* with `v` and `i` declared with the keyword `register`
* with `v` and `i` declared with the keyword `volatile`

Measure the performance with `perf -d` to determine which version performs the best in terms of loop iteration rate.
Which performance counter(s) are relevant for this question?

Explain the semantics of `register` and `volatile` with reference to the generated assembler code and observed experimental results.

### Experiment B: Individual Functional Units

Construct two single-threaded microbenchmarks that exhaust the fixed-point and floating-point execution capacity of your processor core respectively. Ensure that your results are not dominated by dependencies in the instruction stream.

What are the maximum individual fixed-point and floating-point instruction throughputs?


### Experiment C: Overall Throughput

Construct a single-threaded microbenchmark that saturates the backend resources of the core as much as possible.
Experiment with different instruction mixes to determine which combination reaches peak throughput.

What is the resulting instruction throughput?


### Experiment D: SMT Behavior

Construct a multithreaded microbenchmark that runs two non-trivial threads without interference on the same core, i.e. each thread executes with the same instruction throughput as if it were running alone on this core.

For this experiment, you need two create two separate threads using either C++ Threads or PThreads.
It is also necessary to pin the workload to two corresponding logical cpus (hardware threads) on the same core using `numactl --physcpubind=<CPUlist>`.
To find out which logical CPUs correspond to the same core, read `/proc/cpuinfo` and compare the `core id` identifiers.

### Procedure

For each experiment, develop a program `experiment_{a,b_fix,b_flt,c,d}.c`, that can be started without command-line arguments executes the corresponding microbenchmark continuously until it is interrupted by `[Ctrl-C]`.


The endless loop encapsulating the actual microbenchmark (without initialization) should be placed in a function named `benchmark()` for easy recognition (for experiment D use two functions `benchmark1()` and `benchmark2()`).

Provide a corresponding `Makefile` with targets for each experiment.
Also generate and hand in an assembler listing `experiment_{a,b_fix,b_flt,c,d}.s` in the exact same configuration as the experiment binary.
We will not use a validator for this task, due to its platform specific nature.

Perform the experiments and record your results in text files called `experiment_{a,b,c,d}.md`.
Besides the output of `lscpu` and a single *representative* record from `perf -I <interval>`, provide a discussion of your results including which performance counter values are significant for the experiment, and what you deduced from them.
