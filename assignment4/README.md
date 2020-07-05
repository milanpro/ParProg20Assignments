# ParProg20 Assignment 4

*Submission deadline: 2020-07-15, 23:59 CEST*

This assignment covers the development of heterogeneous applications which use FPGAs in shared memory systems. Exemplarily you will use the toolchain and technologies from Xilinx, IBM as well as MetalFS. The tasks should be solved by extending the provided skeleton applications to interact with the MetalFS operator pipeline.


## General Rules

Your solutions have to be submitted at https://osm.hpi.de/submit/.

Our automated submission system is intended to give you feedback about the validity of your file upload. A submission is considered as accepted if the following rules are fulfilled:
* You did not miss the deadline.
* Your file upload can be decompressed with a zip / tar decompression tool.
* Your submitted solution contains only the source code files and a Makefile for Linux. Please leave out any Git clones, backup files or compiled executables.
* Your solution can be compiled using the `make` command, without entering a separate sub-directory after decompression.
* You program runs without expecting any kind of keyboard input or GUI interaction.
* **Our assignment-specific testbench accepts your operator output / generated files.**

If something is wrong, you will be informed via email (console output, error code). Re-uploads of corrected solutions are possible until the deadline.

In order to pass each assignment **50% must be solved correctly**. Documentation should be done inside the source code.

Submit your solutions as team of three persons.


## FPGA Development

For each task, you will develop an FPGA operator using the Metal FS framework.

For information on how to set up your Metal FS development environment, refer to the [lecture material for week 11](https://osm.hpi.de/parProg/) as well as the official [documentation](https://metalfs.github.io).

Use a separate development environment for each task.

We provide skeleton implementations for each task, including the `<operator>.cpp` source file, an `<operator>_tb.cpp` testbench as well as the required `operator.json` and `Makefile`.

You will place your implementation in the `<operator>.cpp` source file.
Optimize your design for throughput and - given the same throughput - prefer designs with a low resource consumption.


## Task 4.1: Heat Map with MetalFS

The heatmap algorithm is the same as in the last assignments, but the implementation mechanics will be different:
* Heatmap cells are unsigned 8-bit integers, where heat values vary from 0 (cold) to 254 (hot).
* Cells holding the value 255 are hotspots and may influence surrounding cells without changing themselves.
* The heatmap operator will calculate exactly one round of the heatmap simulation on the data stream.
* Rows have a fixed width of 64 cells.

Implement the heatmap operator in HLS.
For simplicity, you can expect a fixed `STREAM_BYTES` parameter of 64.

### Input

The operator named `heatmap` expects a binary input stream of heat cells in row order.
The input stream must not contain partial rows, so the stream length is a multiple of 64 bytes.
We provide an example input file in [`heatmap_r0.bin`](task4.1/heatmap_r0.bin).


#### Example call:

    $ od -tx1 -Ax heatmap_r0.bin
      000000 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
      000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
      *
      0000b0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff
      0000c0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
      *
      000150 00 00 00 00 00 00 00 00 f0 00 00 00 00 00 00 00
      000160 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
      *
      000200
    
    $ for I in $(seq $ROUNDS); do
        cat heatmap_r0.bin | /mnt/operators/heatmap > heatmap_r1.bin
        cat heatmap_r1.bin | /mnt/operators/heatmap > heatmap_r0.bin
      done


## Task 4.2: Decrypt (MD5) with FPGAs

Perform a brute-force dictionary attack on a given MD5-hashed password. You need to develop a MetalFS operator that performs the MD5 hashing on a given input string. We implemented the padding mechanism externally, so your operator receives strings which are already padded to 64 bytes. We provide the padding code in `[un]padder.cpp`, an example dictionary [`dict.txt`](task4.2/dict.txt) and a hash to search for in [`hash.bin`](task4.2/hash.bin).

For this task, you can assume a fixed value of 64 for the `STREAM_BYTES` parameter.

### Build

The operator is built using the standard workflow. You only need to build the `padder` and `unpadder` binaries by calling `make padder unpadder`.

### Input

Your operator `md5hash` takes a file containing the binary representation of the hash to search for. The operator expects a data stream of padded 64 byte fixed length strings to calculate the MD5 hash.

#### Example call

    cat dict.txt | ./padder | /mnt/operators/md5hash -s ./hash.bin | ./unpadder
    od -tx1 -Ax hash.bin
	0000000    5d  6c  73  d8  df  ce  f7  14  fe  d8  8f  58  4f  04  e6  3f
	0000010
    
### Output

The operator will return the line of the matching password, or an empty line (`'\0'` characters) if no password matches:

    TrilogyHandinessGallowsUnsignedSponsorPetticoatThinnerE…


## Task 4.3: Colorfilter with FPGAs

Implement a operator named `colorfilter` with HLS, that processes an uncompressed pixel graphic. Your operator should convert all pixels to grayscale, where red is not the *dominant color*. The other pixels are left untouched.

| Example Image         |
| :-------------------: |
| ![](figs/task4.3.png) |

### Input

Extend the skeleton implementation in `colorfilter.cpp`. The input image's RGBA-pixel data is passed as stream to your operator in row order and preceeded by a [BMPv5 file header](https://msdn.microsoft.com/en-us/library/windows/desktop/dd183381(v=vs.85).aspx), which must be kept for the output file.

Be sure to respect the `STREAM_BYTES` parameter, as we might test your design for different data stream widths. You can assume powers of two between 4 and 64.

#### Example call

    /mnt/operators/colorfilter < apples_simulation.bmp > output.bmp

