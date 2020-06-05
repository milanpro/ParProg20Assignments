## Task 2.3: Hashed & Ordered Index with OpenMP

Develop a program, that generates a range of 512 bit blocks, applies the MD5-hash to them and returns the given i-th numerically smallest hash(es).
The first block is generated from a hexadecimal noted byte sequence supplied as the first command line argument. Missing bytes at the block's end are padded with zero. Each following blocks of the block range is incremented by one, acting like a 512-bit big-endian integer.

Your program then applies the MD5-hash to all blocks. Afterwards, the user can query for the i-th smallest hash(es) in the resulting list of hashes.

Both steps, data-generation and search for hashes, should be parallelized using OpenMP. 

### Input

Your executable `hoi` takes three or more arguments: The initial block represented as hexadecimal number, the number of blocks and (multiple) queried indices.

#### Example call

    ./hoi c0ffee 4 0 2

In this example the following blocks and hashes are generated:

    md5(c0ffee00[0...]00) = ec100b976988b81e9465a680afaadecd
    md5(c0ffee00[0...]01) = 210542482275b82b18d71deeca954acf
    md5(c0ffee00[0...]02) = f323610f90a5bf62e51e0f3e44726b9f
    md5(c0ffee00[0...]03) = 1a640827a901ae4c617f6893c16d0398

### Output

The program must terminate with 0 and print line by line the requested smallest hashes. The above example would print the bytes of the smallest and third-smallest of the four hashes (index 0 and 2) in hexadecimal representation:
    
    1a640827a901ae4c617f6893c16d0398
    ec100b976988b81e9465a680afaadecd
