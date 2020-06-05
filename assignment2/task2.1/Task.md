## Task 2.1

Implement a program that simulates heat distribution on a two-dimensional field. The simulation is executed in rounds. The field is divided into equal-sized blocks. Initially some of the blocks are cold (value=0), some other blocks are active hot spots (value=1). The heat from the hot spots then transfers to the neighbor blocks in each of the rounds, which changes their temperature value. 

The new value for each block per round is computed by getting the values of the eight direct neighbor blocks from the last round. The new block value is the average of these values and the own block value from the last round. Blocks on the edges of the field have neighbor blocks outside of the fields, which should be considered to have the value 0. When all block values are computed in a round, the value of the hot spot fields may be set to 1 again, depending on the live time of the hot spot during a given number of rounds. 

Develop a parallel application in C/C++ or Fortran using OpenMP. The goal is to minimize the execution time of the complete simulation.


### Input

Your executable named `heatmap` accepts five parameters:
* The width of the field in number of blocks.
* The height of the field in number of blocks.
* The number of rounds to be simulated.
* The name of a file (in the same directory) describing the hotspots.
* Optional parameter: The name of a file (in the same directory) containing coordinates. If it is passed, only the values at the indicated coordinates (starting at (0, 0) in the upper left corner) are to be written to the output file.

#### Example calls:
    ./heatmap 20 7 17 hotspots.csv
	./heatmap 20 7 17 hotspots.csv coords.csv

The hotspots file has the following structure:
* The first line can be ignored.
* All following lines describe one hotspot per line. The first two values indicate the position in the heat field (x, y). The hot spot is active from a start round (inclusive), which is indicated by the third value, to an end round (exclusive!), that is indicated by the last value of the line.

| Example hotspots.csv      | Example coords.csv |
| ------------------------- | ------------------ |
| `x,y,startround,endround` | `x,y`              |
| `5,2,0,20`                | `5,2`              |
| `15,5,5,15`               | `10,5`             |


### Output

The program must terminate with exit code 0 and has to produce an output file with the name output.txt in the same directory.

If your program was called without a coordinate file, then this file represents the resulting field after the simulation terminated. Each value in the field is encoded the following way:
* A block with a value larger than 0.9 has to be represented as `X`. 
* All other values are incremented by 0.09. From the resulting value, the first digit after the decimal point is added to the output picture.

#### Example content of output.txt without coordinate file

    11112221111111111100
    11123432111111111110
    11124X42211111111111
    11124442111111222111
    11122222111112222211
    11111211111112232211
    01111111111111222111
    
If your program was called with a coordinate file, then this file simply contains a list of exact values requested through the coordinate file, rounded to four decimal points.

#### Example content of output.txt with coordinate file

    1.0000
    0.0306