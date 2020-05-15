## Task 1.2

Implement the dining philosophers with a freely chosen deadlock-free solution strategy using PThread. 

Each philosopher has to be represented by a thread. You are free to map also other stake holders (e.g. waiters) or resources (e.g. forks) to threads if necessary.

#### Input

Your executable has to be named `dinner` and accept two parameters, the number of philosophers resp. forks (min. 3) and the maximum run time in seconds.

#### Example call:

    ./dinner 3 10


### Output

After the given run time is exceeded, the program must terminate with exit code 0 and produce an output file with the name of output.txt in the same directory. This file has to contain nothing but the number of *feedings* per philosopher as semicolon separated values.

#### Example output: 

    5000;5000;5000