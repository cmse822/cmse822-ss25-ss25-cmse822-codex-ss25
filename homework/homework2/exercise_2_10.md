# Exercise 2.10

## Question
Show that, with the scheme for parallel addition just outlined, you can multiply two matrices in $\log_2N$ time with $\frac{𝑁^3}{2}$ processors. What is the resulting efficiency?

## Answer
In a matrix-matrix multiplication of matrices with (N x N), each element is can be calculated by the formula; <br>
$(AB)_{ij} = \sum_{k=1}^{m}A_{ik}B_{kj}$<br>

In this, we can see that each element of the resulting matrix $(AB)$ is independent of any other element in $(AB)$, and as such could be handled as one of $N^2$ parallel operations.

Furthermore, each individual operation is essentially a sum of elements, and could therefore be handled by the same parallel summation algorithm as described in the textbook:

1. Define $s^{(0)}_k = A_{ik}B_{kj}$.
2. Iterate with $l = 1, 2, ... \log_2(N)$
3. Compute $\frac{N}{2^l}$ partial sums: $s^{(l)}_{k} = s^{(l-1)}_{2k} + s^{(l-2)}_{2k+1}$

This operation has a known time of $\log_2 N$ when performed using $\frac{N}{2}$ processors. <br> 
If we provide each of our $N^2$ matrix elements with $\frac{N}{2}$ processors to perform this sum, we get the expected value of $\frac{N^3}{2}$ processors. <br>
If we then assume that there are no startup or discretization costs, then the overall multiplication only takes as long as each operation, so the overall multiplication inherits the $\log_2 N$ time.

To determine the efficiency of this, we will start by finding the speedup. <br>
For the serial version of this function, we would need to perform the sum for every element in the result matrix, resulting in a time of $N^3$. <br>
We determined above that the time of the parallel version is $\log_2N$.

$S_p = \frac{T_s}{T_p} = \frac{N^3}{\log_2{N}}$

From this, we can find the efficiency, where $p$ is the number of processors; <br>
$E = \frac{S_p}{p} = \frac{N^3}{\log_2N}(\frac{2}{N^3}) = \frac{2}{\log_2N}$ <br>

$$ E = \frac{2}{\log_2N}$$