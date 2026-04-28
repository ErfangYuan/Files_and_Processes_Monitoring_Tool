**CSCB09 Winter 2026 Project: System-Wide FD Tables**

**Author: Erfang Yuan**

**Date: March 12, 2026**

**Release/Version: N/A**


**Introduction:**
    This program receives several flagged arguments representing different system scopes, as well as an optional positional argument (representing a specific PID), and outputs a File Descriptor table (including index, process ID, file descriptor, file name, and Inode number) according to the inputted scope. It also outputs a summary table, FD table filtered by threshold, and text/binary files according to specific arguments.

This program can accept command-line inputs in a standard Linux environment and includes the following types:

        --per-process

to indicate that only the process FD table will be displayed

        --systemWide

to indicate that only the system-wide FD table will be displayed

        --Vnode
            
to indicate that the Vnodes FD table will be displayed

        --composite

to indicate that only the composed table (all previous three combined) will be displayed

        --summary

to indicate that a table summarizing number of FDs open per process will be displayed

        --threshold=X

where X denotes an integer, indicating that processes which have larger number of FDs than X should be flagged in the output

        --output_TXT

and/or

        --output_binary

when these flags are used the program will save the "composite" table (i.e. --composite is inputted) in text (ASCII) or binary format into a file named compositeTable.txt or compositeTable.bin respectively

Also, it can accept one integer input 

        [Specific PID] (positional argument)

such as “502606” to represent the specific process (PID) to view. This input must be at the beginning of the command line.


**Problem Solving:**
    The implementation of this program includes the following steps:

1. **Primary Research:** Understand which libraries are required to read PID, FD, File Name (Path) and Inodes information under /proc.

2. **Initial Trial:** Write the parsing logic for command-line arguments (in the main function) and the dependencies of all modules and the logic of each function.

3. **Secondary Research:** Research the specific of implementations for specific tasks (e.g. Dynamic Array for Information CDT to reduce I/O operations; wrapper function for qsort interface).

4. **Basic Implementation:** Based on the above research results, implement complete modules.

5. **Primary Debugging:** Fix syntax errors, modify some printing commands to make sure it has same/similar behaviour as the video representation.

6. **Primary Testing:** Conduct testing with default behaviour, different table arguments, and different threshold/PID inputs.

7. **Final Testing and Reporting:** Fix small behavioral issues and compare the difference of output files for composite table. (Note: the special report for --output_TXT/--output_binary is in the Expected Output Section, Case 5)


**Implementation:**
    The major features (three FD tables and their composite, summary information table, threshold filtered table and the main function) is implemented by standard C libraries and as a separate function to improve clarity and modularity.

**Modules:**
1. **Show Table Module:**

    1.1 **Composite Table Sub-Module:** 
    This sub-module first determines the arguments controllers (special processe, composite table, text output, and binary output), and then reads all true controllers’ and all information from the local copy of FD information summary. Then, according to the auxiliary printing function, it prints line by line into a buffer at the predetermined position; Finally, based on the output controllers, it decides which outputs should be written into from the buffer.

    1.2. **Process Table Sub-Module:**
    This sub-module first determines the arguments controllers (special processe, process table), and then reads all true controllers’ and PID&FD only information from the local copy of FD information summary. Then, according to the auxiliary printing function, it prints line by line into a buffer at the predetermined position and finally writes the buffer into STDOUT.

    1.3. **System-Wide Table Sub-Module:**
    This sub-module first determines the arguments controllers (special processe, process table), and then reads all true controllers’ and PID&FD&File Path information from the local copy of FD information summary. Then, according to the auxiliary printing function, it prints line by line into a buffer at the predetermined position and finally writes the buffer into STDOUT.

    1.4. **Vnode Table Sub-Module:**
    This sub-module first determines the arguments controllers (special processe, process table), and then reads all true controllers’ and FD&Inode information from the local copy of FD information summary. Then, according to the auxiliary printing function, it prints line by line into a buffer at the predetermined position and finally writes the buffer into STDOUT.

    1.5. **Summary Table Sub-Module:**
    This sub-module only works when the summary table controller is true. It first calls the qsort method in the stdlib C library, which accepts a wrapper function pointer comparing by PID in the Info CDT and sorts the local dynamic array; Then it traverses the sorted array, where the files with same PID forms a process block, allowing it to use a single counter to record the number of files in each process block.

    1.6. **Threshold Table Sub-Module:**
    This sub-module only works when the summary table controller is true. It first calls the qsort method in the stdlib C library, which accepts a wrapper function pointer comparing by PID in the Info CDT and sorts the local dynamic array; Then it traverses the sorted array, where the files with same PID forms a process block, allowing it to use a single counter to record the number of files in each process block; after counting each process block, it checks whether the total number of FDs associated with that PID exceeds the given threshold, and prints the process information if so.


2. **Create Array Module:**
This module reads /proc by opening the directory of proc (based on <dirent.h>) to get PID with its type declared in <sys/types.h> as well as the FD directory under this PID, then concatenates the paths and call the readlink interface under <unistd.h> to read the file path (name), and finally gets the inode (File serial number) from the stat struct under <sys/stat.h>. It will then load all FD related information into a dynamic array (an ADT containing capacity and size), where each element of the array is a CDT that records individual file information.

3. **Argument Parsing Module (main):**
This main function receives command-line arguments and identifies positional arguments, as well as parsing flagged arguments; Based on the input arguments, it prompts error messages or mark the corresponding controller as true, finally, all the above modules are called.


**Information Flow:**
    <div align="center">
    <img src="res\graphs\flow_chart.png" width="800" height="600">
    </div>


**File Compiling:**
To compile the file, you need to run the following steps on a **Linux** kernel device or simulated environment.

First, you need to confirm the existence of all modules and the Makefile by using shell command **ls**. You should be expected to see: 

```python
common.h

showtables.h

create_array.c

showtables.c

main.c

Makefile
```

Then clean the existed files by inputting the command:

        make clean
You should be expected to see:

        rm -f *.o showFDtables

Now compile these modules by inputting the command:

        make
You should be expected to see:

        gcc -Wall -g -Werror -c main.c
        gcc -Wall -g -Werror -c showtables.c
        gcc -Wall -g -Werror -c create_array.c
        gcc -Wall -g -Werror -o showFDtables main.o showtables.o create_array.o


**Expected Output:**
    Use the command:

        ./showFDtables

to open the executable file to view the default behaviour, and use

        ./showFDtables [Specific PID] [--per-process] [--systemWide] [--Vnode] [--composite] [--summary] [--threshold=X] [--output_TXT] [--output_binary]

to test with positional arguments and/or flagged arguments.

The information presented by the monitoring tool should be directed to STDOUT and will include the corresponding graph, for example:

1. If input is

    ```python
    ./showFDtables 
    ```
    then the default output is to print the composite table of all processes (since no specific processes inputted):
    
    <div align="center"><img src="res\graphs\expected_output.1.1.png" width="800" height="600"></div>

    ...

    some files omitted

    ...

    <div align="center">
    <img src="res\graphs\expected_output.1.2.png" width="800" height="600">
    </div>

2. If input includes positional arguments only, e.g.


    ```python
    ./showFDtables 502606
    ```

    in this case the default behaviour will be applied on the PID (i.e. positional argument) you specified, so the composite table for this specific process will be printed:

    <div align="center">
    <img src="res\graphs\expected_output.2.png" width="800" height="600">
    </div>

3. If input includes mixed positional argument and flagged arguments, e.g.

    ```python
    ./showFDtables --composite
    ```

    with the no process specified, then the composite table will be shown:
        
    Note this is equivalent to the default behaviour, so please refer to Expected Output Case 1.

4. If input includes mixed positional argument and flagged arguments, e.g.

    ```python
    ./showFDtables 502606 --Vnode
    ```

    with the same process specified from Expected Output Case 2, only the Vnode table will be shown:

    <div align="center">
    <img src="res\graphs\expected_output.4.png" width="800" height="600">
    </div>

5. If you want to see the output of tables (note this program only supports you to output the composite for either the specific process or all processes in text/binary formats), you MUST include --composite as one of the flagged argument and --output_TXT and/or --output_binary as the output controller:

    For example, if your input is:

    ```python
    ./showFDtables 502606 --composite --output_TXT --output_binary
    ```

    It should output two files that contain the composite table for PID = 502606, named compositeTable.txt and compositeTable.bin respectively.

    You can verify the above files by shell command **ls**.
        
    **Research Procedure:**
        Here we expected to see some difference/similarities between these two outputs: We choose the PID 502606 as the specific PID as the experiment condition, whereas all processes (i.e. without the positional argument) is the control condition. Then we perform 5 repeated measurements of the time and size of two files written for each condition, and calculate their mean and standard deviation to seek potential relationships. (i.e. For conditions with/without the positional argument, use shell command: 

        time

        du -b compositeTable.

    respectively to obtain the duration of file written and size of the file output.)

    (Note: we will use the real time for calculation.)


    **Result:**
    <div style="padding-left: 40px;">
        Experiment Condition:
    </div>

    <div style="padding-left: 60px;">
        Timing:

        1.  
        time ./showFDtables 502606 --composite --output_TXT

        real 0m0.014s

        time ./showFDtables 502606 --composite --output_binary

        real 0m0.014s

        2.  
        time ./showFDtables 502606 --composite --output_TXT

        real 0m0.014s
                
        time ./showFDtables 502606 --composite --output_binary

        real 0m0.014s

        3.  
        time ./showFDtables 502606 --composite --output_TXT

        real 0m0.013s

        time ./showFDtables 502606 --composite --output_binary

        real 0m0.013s

        4.  
        time ./showFDtables 502606 --composite --output_TXT

        real 0m0.016s
                    
        time ./showFDtables 502606 --composite --output_binary

        real 0m0.017s

        5.  
        time ./showFDtables 502606 --composite --output_TXT

        real 0m0.014s

        time ./showFDtables 502606 --composite --output_binary

        real 0m0.014s
    </div>

    <div style="padding-left: 60px;">
        Size:

        1.  
        ./showFDtables 502606 --composite --output_TXT --output_binary
                    
        du -b compositeTable.*

        2996 compositeTable.bin
        2996 compositeTable.txt

        2.  
        ./showFDtables 502606 --composite --output_TXT --output_binary

        du -b compositeTable.*

        2996 compositeTable.bin
        2996 compositeTable.txt

        3.  
        ./showFDtables 502606 --composite --output_TXT --output_binary

        du -b compositeTable.*

        2996 compositeTable.bin
        2996 compositeTable.txt

        4.  
        ./showFDtables 502606 --composite --output_TXT --output_binary

        du -b compositeTable.*

        2996 compositeTable.bin
        2996 compositeTable.txt

        5.  
        ./showFDtables 502606 --composite --output_TXT --output_binary

        du -b compositeTable.*

        2996 compositeTable.bin
        2996 compositeTable.txt
    </div>

    <div style="padding-left: 40px;">
        Control Condition:
    </div>

    <div style="padding-left: 60px;">
        Timing:

        1.  
        time ./showFDtables --composite --output_TXT

        real 0m0.016s

        time ./showFDtables --composite --output_binary

        real 0m0.013s

        2.  
        time ./showFDtables --composite --output_TXT

        real 0m0.014s

        time ./showFDtables --composite --output_binary

        real 0m0.015s

        3.  
        time ./showFDtables --composite --output_TXT

        real 0m0.014s

        time ./showFDtables --composite --output_binary

        real 0m0.013s

        4.  
        time ./showFDtables --composite --output_TXT

        real 0m0.014s

        time ./showFDtables --composite --output_binary

        real 0m0.023s

        5.  
        time ./showFDtables --composite --output_TXT

        real 0m0.014s
                        
        time ./showFDtables --composite --output_binary
                        
        real 0m0.014s
    </div>

    <div style="padding-left: 60px;">
        Size:

        1. 
        ./showFDtables --composite --output_TXT --output_binary

        du -b compositeTable.*

        11356 compositeTable.bin
        11356 compositeTable.txt

        2.  
        ./showFDtables --composite --output_TXT --output_binary

        du -b compositeTable.*

        11356 compositeTable.bin
        11356 compositeTable.txt

        3. 
        ./showFDtables --composite --output_TXT --output_binary

        du -b compositeTable.*

        11356 compositeTable.bin
        11356 compositeTable.txt

        4.  
        ./showFDtables --composite --output_TXT --output_binary

        du -b compositeTable.*

        11356 compositeTable.bin
        11356 compositeTable.txt

        5.  
        ./showFDtables --composite --output_TXT --output_binary

        du -b compositeTable.*
        11356 compositeTable.bin
        11356 compositeTable.txt
    </div>

    **Analysis:**
    <div style="padding-left: 20px;">

    **Experiment Condition:**

        Output Type | Mean Time(s) | Std Dev(s) | Mean Size(bytes) | Std Dev(bytes)
        TXT             0.0142          0.0011          2996            0
        Binary          0.0144          0.0015          2996            0

    **Control Condition:**

        Output Type | Mean Time(s) | Std Dev(s) | Mean Size(bytes) | Std Dev(bytes)
        TXT             0.0144          0.0009          11356           0
        Binary          0.0156          0.0042          11356           0

    The experiment results show that the text and binary output files have identical file sizes in both conditions. This is because the program stores all file and process information as big character buffer (i.e. a long string), and so it writes the buffer with exactly the same ASCII value for each character into both files, and the binary output does not store structured binary data but simply writes the formatted ASCII byte stream.

    The execution times between text and binary outputs are also very similar. The mean differences are on the order of 0.0002–0.0012 seconds.

    However, within larger byte streams, changes in individual files (outliers) may lead to significant changes in execution time, resulting in a larger standard deviation of the Control Condition. Due to the low sample size, we are unable to determine whether this gap is significant.
    </div>

    **Conclusion:**
        This experiment demonstrates that binary files and txt files store the same ASCII byte stream, so the space cost of I/O is consistent, and the time cost should also be of similar magnitude. This is because the writing process of this buffer only depends on different modes ("w" or "wb") of the same I/O operation (fwrite). (See more details in the Composite Table Sub-Module of Show Table Module under the Implementation Section as well as the program code.)


**Test Cases:**
    For basic tests, like testing the default behaviour, positional arguments only, flagged arguments only, mixed positional arguments and flagged arguments, please refer to Expected Output Section.

1. Edge Cases (Misuse):

    Case 1.1:
    If you made a typo so that input an invalid argument, a prompt will be shown and no tables will be shown:<div align="center"><img src="res\graphs\test_cases.1.1.png" width="800" height="40"></div>
    Case 1.2: 
    If you mistakenly input an illegal positional argument (PID), you will be prompted an error message and no tables will be shown:<div align="center"><img src="res\graphs\test_cases.1.2.png" width="800" height="40"></div>
    Case 1.3:
    If you want to see the output for the composite table, however, you forget to flag the ```
    --composite``` argument, it will not show any tables or output anything and prompt an error message:<div align="center"><img src="res\graphs\test_cases.1.3.png" width="800" height="40"></div>


2. Advanced Tests (From A2 handout and/or Video Demonstration):

    Case 2.1:
    If the input is:

        ./showFDtables --composite

    Output:
        Please refer to Expected Output Section Case 3
                
    Case 2.2:
    If we want to see consecutive tables for a specific PID, e.g. when you input:

        ./showFDtables 502606 --per-process --systemWide --Vnode --composite

    Output:<div align="center"><img src="res\graphs\test_cases.2.2.1.png" width="800" height="600"><img src="res\graphs\test_cases.2.2.2.png" width="800" height="600"><img src="res\graphs\test_cases.2.2.3.png" width="800" height="600"><img src="res\graphs\test_cases.2.2.4.png" width="800" height="200"></div>

    As for the order of the tables, please refer to the Disclaimer Section.


    Case 2.3:
    If you also want to see the summary table that contains the PID and how many files it contains, you can input:

        ./showFDtables 502606 -per-process --systemWide --Vnode --composite --summary

    Based on Case 2.2 (Note the summary table is a global behaviour that does not related to the PID you specified).

    Output:
        Everything same (or similar, since some processes or files might changed between each test) as Test Case 2.2, with one additional table:
        <div align="center"><img src="res\graphs\test_cases.2.3.png" width="800" height="40"></div>

    Case 2.4:
    If you want to know if there exists a specific FD such that it is greater than some value, you can input:

        ./showFDtables --summary --threshold=15

    where 15 in this case is the value we want. (Here the the input is different because this test is not completed at the same time as previous tests. And the summary table could differ.)

    Output:<div align="center"><img src="res\graphs\test_cases.2.4.png" width="800" height="60"></div>

    Case 2.5:
    Based on Case 2.4, if we adjust the threshold to a higher value, e.g.:

        ./showFDtables --summary --threshold=50

    Output:<div align="center"><img src="res\graphs\test_cases.2.5.png" width="800" height="60"></div>


**Disclaimer:**
1. Please follow the established validly instructed input and the compiling steps, which is detailed in the File Compiling Section and Expected Output Section. Improper behavior caused by invalid instructions will be prompted as shown in Test Cases Section.

2. The position argument (i.e. specific PID) MUST be input as the FIRST command-line argument, otherwise it will not be parsed.

3. When multiple tables are expected to be printed, they will follow the following order:
    1. coposite table
    2. per-process table
    3. system wide table
    4. Vnode table

    This order is fixed and independent of the order of command-line arguments.

    This program does not explicitly handle the printing order of tables, they are actually printed asynchronously one after another (but appear synchronous due to extremely short runtime).

4. The process information that this program can handle is not infinite, as it does not use dynamic storage of this information being written. This is to reduce the cost of information insertion and memory release. The information capacity of this program is hard coded, where the length of a single file name (path) should not exceed 2048 (2^11) characters, and the total length of all information (index, PID, FD, file name, and Inode) (as they are continuously written into the buffer) should not exceed 1048576 (2 ^ 20) characters. Exceeding the range of information storage can lead to array overflow. This program assumes that the process information being tested on an average OS should be less than the hard coded capacity, and therefore is not responsible for undefined behavior of array overflow.

5. Specifically, this program does not count the following five situations when reading FD information from/proc:
    1. Files without PID (non process)
    2. Process unable to obtain FD (process terminates at this time or does not have permission to access FD)
    3. Repeatedly read FD or FD's parent directory
    4. Unable to obtain the path of the file to which FD belongs (the process terminates at this time or does not have permission to obtain the path)
    5. Unable to obtain the Inode of the file to which FD belongs (the process terminates at this time or does not have permission to obtain Inode)

    The first three situations are more reasonable, while the latter two are controversial as they may affect the statistics of the number of files in the process (i.e. the performance of the summary table and threshold table). However, because the return values of readlink or stat cannot determine whether the process has terminated or does not have permission to open at this time, this program skips the latter two situations and only counts the files that can be specifically determined. At the same time, the process may terminate during the reading process, the actual statistical results could to be inconsistent with the actual process behavior with very little probability, and so this program cannot truly perform real-time statistics. But testers can run this program multiple times to check for process differences.


**References:**

[sys_stat.h(0p) - Linux manual page](https://man7.org/linux/man-pages/man0/sys_stat.h.0p.html)

[unistd.h(0p) - Linux manual page](https://man7.org/linux/man-pages/man0/unistd.h.0p.html)

[dirent.h(0p) - Linux manual page](https://man7.org/linux/man-pages/man0/dirent.h.0p.html)

[proc_pid_fd(5) - Linux manual page](https://man7.org/linux/man-pages/man5/proc_pid_fd.5.html)

https://en.wikipedia.org/wiki/Dynamic_array

https://en.wikipedia.org/wiki/Qsort

[Piazza](https://piazza.com/class/mjc0lbx4crw55d#)

https://bitlaunch.io/blog/linux-file-size-bytes/



        

    
    




            