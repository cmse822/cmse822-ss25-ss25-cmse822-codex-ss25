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
For part 1 the `ert toolkit` ran on `intel-16`, `intel-18` and `amd-20` with base configurations.

![Intel 18](https://github.com/user-attachments/assets/dbb9a7a4-ca5f-46ea-9f12-5208a6b5681e)
![Intel 16](https://github.com/user-attachments/assets/8740852d-4897-4959-bfaf-39dd18d2dc08)
![AMD 20](https://github.com/user-attachments/assets/a239edbd-ba3d-4f90-ab5e-9fa0fa87eccf)

### - 3. _Performance Ridge Point_

| Metric               | AMD 20 | Intel 16 | Intel 18 |
| ---                  | ---    | ---      | ---      |
| L1 Bandwidth (GB/s)  | 206.2  | 172.4    | 242.7    |
| L2 Bandwidth (GB/s)  | 174.3  | 113.5    | 174.5    |
| L3 Bandwidth (GB/s)  | 41.4   | 71.2     | 83.1     |
| Peak Perf (GFLOPS/s) | 40.0   | 31.4     | 47.3     |
| L1 Ridge             | ~0.2   | ~0.2     | ~0.2     |
| L2 Ridge             | ~0.4   | ~0.3     | ~0.3     |
| L3 Ridge             | ~1     | ~0.4     | ~0.6     |

