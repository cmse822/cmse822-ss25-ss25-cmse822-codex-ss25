# Using VTune

To use Intel VTune, make sure you are in one of the Intel dev nodes

From there you can type 'vtune' into the HPCC terminal to get a list of instructions. It prints a whole lot, so there is a lot to take in here. The most basic instruction you can give it probably just "vtune -collect performance-snapshot ./your_code". Note that, if using c++ code you'll need to compile it first with a command like "g++ your_code.cpp -o your_code".

The matrix sample can be found in https://github.com/oneapi-src/oneAPI-samples it's in the oneAPI-samples/Tools/VTuneProfiler/matrix_multiply_c directory.

# Part 1
Arthemetic intensity of the kernel calculation:
- Y[j] += Y[j] + A[j][i] * B[i]
        - Two additions and one multiplication = 3 FLOPS, accesses Y[j], A[j][i], and B[i] once, so 3 memory accesses (8 bytes each). So: $AI=\frac{3 FLOPS}{3\times 8 bytes} = \frac{1}24{} \frac{FLOPS}{byte}$
- s += A[i] * A[i]
        - $\frac{2}{1\times 8} = \frac{1}{4} \frac{FLOPS}{byte}$
- s += A[i] * B[i]
        - $\frac{2}{1\times 8} = \frac{1}{4} \frac{FLOPS}{byte}$
- Y[i] = A[i] + C*B[i]
        - $\frac{2}{3\times 8} = \frac{1}{12} \frac{FLOPS}{byte}$

# Part 2

================= PERFORMANCE REPORT =================
Timer Name              Total Time (s)     Calls     Avg (ms)Mega Zone Updates/s
--------------------------------------------------------------------------
main                          0.249794         1   249.794311            1.88
timeLoop                      0.237587         1   237.587165            1.98
EulerSolve                    0.228685       919     0.248842            2.06
computeL                      0.176841      1838     0.096214            2.66
--------------------------------------------------------------------------

# Part 3

I cannot get the Gravity Collapse program to run, there is a malloc error but I do not remember/know how to solve it.

# Part 4

Using the following command:

```bash
$ vtune -collect hotspots -result-dir Sod_Shock ./agoge_run problems/SodShockTube.yaml
```

I was able to get the following from VTune:

```
CPU Time: 0.210s
        Effective Time: 0.210s
        Spin Time: 0s
        Overhead Time: 0s
    Total Thread Count: 1
    Paused Time: 0s
```

and:

Top Hotspots
Function                       Module     CPU Time  % of CPU Time(%)
-----------------------------  ---------  --------  ----------------
agoge::Field3D::applyBCs       agoge_run    0.114s             54.3%
agoge::euler::computeL         agoge_run    0.048s             22.9%
__memset_avx2_unaligned_erms   libc.so.6    0.020s              9.5%
agoge::euler::computeTimeStep  agoge_run    0.020s              9.5%
agoge::euler::runRK2           agoge_run    0.008s              3.8%

