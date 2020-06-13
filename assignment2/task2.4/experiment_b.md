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

In this experiment, the number of instructions is the relevant 
metric as both benchmarks mainly consist of either fixed or floating point 
operations. Because of this, the measured instructions should be 
very similar to the fixed-point and floating-point instruction throughputs
we want to measure. There might be more specialized performance counters for
this purpose but we didn't find them.

Fixed point:

    47.018451304           1.000,29 msec task-clock                #    1,000 CPUs utilized          
    47.018451304                  2      context-switches          #    0,002 K/sec                  
    47.018451304                  0      cpu-migrations            #    0,000 K/sec                  
    47.018451304                  0      page-faults               #    0,000 K/sec                  
    47.018451304      2.494.872.802      cycles                    #    2,494 GHz                    
    47.018451304      6.721.627.064      instructions              #    3,13  insn per cycle         
    47.018451304        269.007.202      branches                  #  268,957 M/sec                  
    47.018451304              8.968      branch-misses             #    0,00% of all branches 

Floating point:

    19.006238652             999,85 msec task-clock                #    1,000 CPUs utilized          
    19.006238652                 17      context-switches          #    0,017 K/sec                  
    19.006238652                  1      cpu-migrations            #    0,001 K/sec                  
    19.006238652                  0      page-faults               #    0,000 K/sec                  
    19.006238652      2.371.404.450      cycles                    #    2,374 GHz                    
    19.006238652      5.180.222.408      instructions              #    2,01  insn per cycle         
    19.006238652        157.138.984      branches                  #  157,290 M/sec                  
    19.006238652             10.467      branch-misses             #    0,01% of all branches 
