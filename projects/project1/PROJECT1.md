# Project 1

## Warm Up
Below is the table with the example kernels and their arithmetic intensities in FLOPs/byte:

| Kernel No. | Kernel Operation                 | Arithmetic Intensity        |
| ---        | ---                              | ---                         |
| 1          | `Y[j] += Y[j] + A[j][i] * B[i]`  | $\frac{3}{32}$ = 0.09375    |
| 2          | `s += A[i] * A[i]`               | $\frac{1}{4}$ = 0.25        |
| 3          | `s += A[i] * B[i]`               | $\frac{1}{8}$ = 0.125       |
| 4          | `Y[i] = A[i] + C*B[i]`           | $\frac{1}{12}\approx 0.0833$|
- Kernel 1 on each iteration requires 3 loads (`Y[j]`, `A[j][i]`, `B[i]`), 1 store, and 3 FLOPs. So, the arithmetic intensity is 3/(4*8) = 3/32.
- In kernel 2, `s` can be kept in an accummulator register and only be written once at the end of the loop, so it can be excluded from the arithmetic intensity calculation. Thus, each iteration requires 1 load (`A[i]`) and 2 FLOPs, so the AI is 2/8 = 1/4.
- Kernel 3 is similar to kernel 2 with an extra load, so the AI is 2/16 = 1/8.
- In kernel 4, `C` is a scalar which can be kept in register. Then, it requires 2 loads (`A[i]` and `B[i]`), 1 store, and 2 FLOPs. So, the AI is 2/(3*8) = 1/12.

## Part 1
Below are the results from running `ert toolkit` on `intel-16`, `intel-18` and `amd-20` with base configurations.

![Intel 18](https://github.com/user-attachments/assets/dbb9a7a4-ca5f-46ea-9f12-5208a6b5681e)
![Intel 16](https://github.com/user-attachments/assets/8740852d-4897-4959-bfaf-39dd18d2dc08)
![AMD 20](https://github.com/user-attachments/assets/a239edbd-ba3d-4f90-ab5e-9fa0fa87eccf)

We summarize the plots in the table below, including ridge points for each cache level:

| Metric               | AMD 20 | Intel 16 | Intel 18 |
| ---                  | ---    | ---      | ---      |
| L1 Bandwidth (GB/s)  | 206.2  | 172.4    | 242.7    |
| L2 Bandwidth (GB/s)  | 174.3  | 113.5    | 174.5    |
| L3 Bandwidth (GB/s)  | 41.4   | 71.2     | 83.1     |
| Peak Perf (GFLOPS/s) | 40.0   | 31.4     | 47.3     |
| L1 Ridge             | ~0.2   | ~0.2     | ~0.2     |
| L2 Ridge             | ~0.4   | ~0.3     | ~0.3     |
| L3 Ridge             | ~1     | ~0.4     | ~0.6     |

#

Now, we can estimate performance of the kernels from "Roofline: An Insightful Visual..." which include SpMV, LBMHD, Stencil, and 3-D FFT. Below are plots of the Intel18 and AMD20 roofline models with lines added for the high end of these kernels' arithmetic intensities, using the following key:
- SpMV ---- Green
- LBMHD -- Red
- Stencil --- Blue
- 3-D FFT -- Purple

![Intel18 Roofline](./Roofline_Images/intel18_Roofline_Kernels.png)

__SpMV__ hits peak performance for L1 only, this kernel would have mediocre performance on this system since it is unlikely for the problem to fit entirely in L1. It hits the L2 roofline close to peak performance, but if the problem is big enough to require L3 it would take a performance hit of around 20 GLOPS/sec.

__LBMHD__ hits peak performance for L2, so this kernel would perform better. If L3 is required a small performance hit would be suffered but if DRAM is required it will take another large decrease of around 20 GFLOPS/sec.

__Stencil__ hits peak performance for L3, so this kernel would perform well. If DRAM is required there will be roughly a 5 GFLOPS/sec performance decrease, but this is manageable.

__3-D FFT__ has a high enough arithmetic intensity to hit peak performance on this system for all levels of memory. The only thing to be careful of in this case is page faults.

![AMD20 Roofline](./Roofline_Images/amd20_Roofline_Kernels.png)

Looking at the AMD system, we see a similar story. The main differences are
- __SpMV__ hits peak performance for L2 cache on this system.
- L3 cache is not captured on this system.
- __Stencil__ hits peak performance for DRAM on this system.

It is also worth noting that the peak performance on the AMD20 is about 7 GLFOPs/sec slower than the Intel18.

#

Now, we will do similar analysis for the kernels from the warm-up using the following key:
- Kernel 1 -- Green
- Kernel 2 -- Red
- Kernel 3 -- Blue
- Kernel 4 -- Purple

![Intel18 Warmup](./Roofline_Images/intel18_Warmup_Kernels.png)

__Kernels 1, 3, and 4__ all fail to hit peak performance on any memory level due to having relatively low arithmetic intensities. Thus, they will all perform relatively poorly because they will not be able to make use of the available bandwidth. For instance, __Kernel 4__ has the worst performance hitting the L1 roofline at around 20 GFLOPs/sec and the DRAM roofline as low as around 3 GFLOPS/sec. The best of these three, __Kernel 3__, still only reaches the 30 GFLOPs/sec point on the L1 roofline when peak performance is around 47 GFLOPs/sec.

__Kernel 2__ fares the best, having the same arithmetic intensity as the previously discussed __SpMV__. Notably, that kernel had the worst expected performance in the last bunch, emphasizing the low intensity of Kernels 1-4. These kernels' performance can likely be improved marginally by using optimizations like prefetching, but ultimately they are highly latency-bound.

![AMD20 Warmup](./Roofline_Images/amd20_Warmup_Kernels.png)

On the AMD20 system, we would still expect poor performance from __Kernels 1, 3, and 4__, hitting L1 roofline at best around 25 GFLOPs/sec and at worst around 18 GFLOPs/sec. __Kernel 2__ Hits peak performance for L2 cache, but hits the DRAM roofline at around 10 GLFOPs/sec.
