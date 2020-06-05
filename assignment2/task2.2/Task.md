## Task 2.2: Decrypt with OpenMP

Develop an OpenMP-based command line tool named `decrypt` that performs a brute-force dictionary attack on Unix [`crypt(3)`](https://www.man7.org/linux/man-pages/man3/crypt.3.html) passwords.

An example password file to be attacked is available at [`taskCryptPw.txt`](./taskCryptPw.txt).
Each line of the password file contains the username and the encrypted password, separated by the character `:`. 

Your program can use the example dictionary file available at [`taskCryptDict.txt`](./taskCryptDict.txt).
One of the users has a password exactly matching one dictionary entry. A second user has a password build from one of the dictionary entries plus a single number digit (0-9) attached, e.g. `Abakus5`.

It is recommended to start with a serial version add the OpenMP parallelization as the last step.

Please note that the first two characters of the encrypted password string in `taskCryptPw.txt` are the salt string used in the original encryption process. A correct solution therefore splits the encrypted password string into salt and encryption payload, calls some `crypt(3)` implementation with the salt and all of the dictionary entries, and checks if one of the crypt results matches with an entry from the user list.
Input

### Input

Your executable `decrypt` takes two arguments: the name of the password file as and the name of the dictionary file.

#### Example call

    ./decrypt taskCryptPw.txt taskCryptDict.txt
    
### Output

The program must terminate with exit code 0 and print a line by line a list of the successfully cracked combinations of username and decrypted password, both separated by semicolon:

    User01;pass
    User02;Abakus5
    
(This is not the solution). Submit a compressed archive with the OpenMP sources. Additionally, the archive can also contain a file named `taskCryptSolution.txt` with the cracked users for the above example data (include a trailing new-line). In this case the validation step will tell you if you found the right ones. The validation machine will not crack the example data, since this may take several hours.