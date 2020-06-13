lscpu output:

    Architecture:                      x86_64  
    CPU op-mode(s):                    32-bit, 64-bit  
    Byte Order:                        Little Endian  
    Address sizes:                     39 bits physical, 48 bits virtual  
    CPU(s):                            8  
    On-line CPU(s) list:               0-7  
    Thread(s) per core:                2  
    Core(s) per socket:                4  
    Socket(s):                         1  
    NUMA node(s):                      1  
    Vendor ID:                         GenuineIntel  
    CPU family:                        6  
    Model:                             142  
    Model name:                        Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz  
    Stepping:                          10  
    CPU MHz:                           800.069  
    CPU max MHz:                       3400,0000  
    CPU min MHz:                       400,0000  
    BogoMIPS:                          3600.00  
    Virtualization:                    VT-x  
    L1d cache:                         128 KiB  
    L1i cache:                         128 KiB  
    L2 cache:                          1 MiB  
    L3 cache:                          6 MiB  
    NUMA node0 CPU(s):                 0-7  
    Vulnerability Itlb multihit:       KVM: Mitigation: Split huge pages  
    Vulnerability L1tf:                Mitigation; PTE Inversion; VMX conditional cache flushes, SMT vulnerable  
    Vulnerability Mds:                 Mitigation; Clear CPU buffers; SMT vulnerable  
    Vulnerability Meltdown:            Mitigation; PTI  
    Vulnerability Spec store bypass:   Mitigation; Speculative Store Bypass disabled via prctl and seccomp  
    Vulnerability Spectre v1:          Mitigation; usercopy/swapgs barriers and __user pointer sanitization  
    Vulnerability Spectre v2:          Mitigation; Full generic retpoline, IBPB conditional, IBRS_FW, STIBP conditional, RSB filling  
    Vulnerability Tsx async abort:     Not affected  
    Flags:                             fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art   
                                       arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 s  
                                       se4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vn  
                                       mi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida  
                                        arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d

Here, the number of branches is the metric to watch, because the only 
branch in the code is the loop condition, which means that each branch taken
is equivalent to one loop iteration.

Default:

    11.002861583           1.000,21 msec task-clock                #    1,000 CPUs utilized          
    11.002861583                  5      context-switches          #    0,005 K/sec                  
    11.002861583                  0      cpu-migrations            #    0,000 K/sec                  
    11.002861583                  0      page-faults               #    0,000 K/sec                  
    11.002861583      3.391.685.408      cycles                    #    3,391 GHz                    
    11.002861583      3.117.240.261      instructions              #    0,92  insn per cycle         
    11.002861583        346.422.705      branches                  #  346,352 M/sec                  
    11.002861583              7.435      branch-misses             #    0,00% of all branches     

With `register` keyword

    20.005350125           1.000,24 msec task-clock                #    1,000 CPUs utilized          
    20.005350125                  3      context-switches          #    0,003 K/sec                  
    20.005350125                  0      cpu-migrations            #    0,000 K/sec                  
    20.005350125                  0      page-faults               #    0,000 K/sec                  
    20.005350125      3.392.235.278      cycles                    #    3,391 GHz                    
    20.005350125      3.954.740.165      instructions              #    1,17  insn per cycle         
    20.005350125        564.991.420      branches                  #  564,863 M/sec                  
    20.005350125              7.223      branch-misses             #    0,00% of all branches

With `volatile` keyword

    11.002772361           1.000,15 msec task-clock                #    1,000 CPUs utilized          
    11.002772361                  3      context-switches          #    0,003 K/sec                  
    11.002772361                  1      cpu-migrations            #    0,001 K/sec                  
    11.002772361                  0      page-faults               #    0,000 K/sec                  
    11.002772361      3.391.944.751      cycles                    #    3,391 GHz                    
    11.002772361      4.431.216.954      instructions              #    1,31  insn per cycle         
    11.002772361        340.963.505      branches                  #  340,906 M/sec                  
    11.002772361              7.001      branch-misses             #    0,00% of all branches 

We see that the normal version and the one with the `volatile` keyword
are pretty similar in speed, while the execution is much faster with the 
`register` keyword. This is due to `register` recommending the compiler to 
store the variable in a register as opposed to main memory. In this 
experiment, this results in a massive speedup because `v` and `i` are used
very frequently. The `volatile` keyword on the other hand discourages the 
compiler from applying such optimizations by indicating that the variable
may be changed without being accessed in the source code. In this instance, 
the performance is similar to the standard variant, because that does not 
apply these optimizations either. Howver, if we were to compile with more 
aggressive optimization flags such as `-O3`, we might see `volatile` be even 
slower.
