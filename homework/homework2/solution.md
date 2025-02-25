# Solutions

**Team**: Thread Titans

**Team Members**: Alex, Bashar, Fizza, Maya, Ramin

## Exercise 2.3

Assume that an addition takes a certain unit time, and that moving a number from one processor to another takes that same unit time. Show that the communication time equals the computation time. 

Now assume that sending a number from processor $p$ to $p \pm k$ takes time $k$. Show that the execution time of the parallel algorithm now is of the same order as the sequential time.

Answer:

## Exercise 2.5

Answer

## Exercise 2.6

Prove that $E = 1$ implies that all processors are active all the time. (Hint: suppose all processors finish their work in time $T$ , except for one processor in $T'$ < $T$ . What is $T_p$ in this case? Explore the above relations.)

Answer: Assume, for contradiction, that $E = 1$ but not all processors are active at all times. Efficiency is defined as  

$$ E_p = \frac{S_p}{p} = \frac{T_1 / T_p}{p} $$  

If $E_p = 1$, then  

$$ \frac{T_1 / T_p}{p} = 1 \implies T_p = \frac{T_1}{p} $$  

which represents perfect parallelization where all processors finish simultaneously. Suppose one processor finishes early at $T' < T_p$, while the slowest processor takes $T_p$. Then, the system's execution time remains $T_p$, but at least one processor is idle for  

$$ \Delta T = T_p - T' $$  

Since efficiency is based on useful work, the presence of idle processors means the effective work done is strictly less than $T_p \cdot p$, leading to  

$$ E_p = \frac{T_1}{T_p \cdot p} < 1 $$  

which contradicts the assumption that $E_p = 1$. Therefore, all processors must be active for the entire duration.  

## Exercise 2.10

Show that, with the scheme for parallel addition just outlined, you can multiply two matrices in $\log_2 N$ time with $N^3/2$ processors. What is the resulting efficiency?

Answer: The standard matrix multiplication algorithm for two $N \times N$ matrices requires $O(N^3)$ operations. Each element $C_{ij}$ in the result matrix is computed as:

$$C_{ij} = \sum_{k=1}^{N} A_{ik} B_{kj}$$

which involves summing $N$ terms. A naive approach assigns $N^3$ processors, each computing a single multiplication, resulting in an initial step of $O(1)$ time. However, summation, the bottleneck still takes $O(n)$ time.

Using the parallel sum strategy, each sum can be computed in $O(\log_2 N)$ time using $N/2$ processors, as each step halves the number of active processors. Since there are $N^2$ independent summations (one per entry in $C$), the total number of processors required for summation is:

$$\frac{N^2 \cdot N}{2} = \frac{N^3}{2}$$

Thus, with $\frac{N^3}{2}$ processors, the total time for matrix multiplication is determined by the longest step, which is the summation step, giving a parallel time of:

$$T_P = O(\log_2 N)$$

Efficiency is given by:

$$E = \frac{\text{sequential time}}{\text{parallel time} \times \text{number of processors}}$$

Since the sequential execution time is $O(N^3)$, and the parallel time is $O(\log_2 N)$ with $O(N^3/2)$ processors, we obtain:

$$E = \frac{O(N^3)}{O(\log_2 N) \times O(N^3/2)}$$

$$E = O\left(\frac{N^3}{(N^3/2) \log_2 N}\right) = O\left(\frac{2}{\log_2 N}\right)$$

The efficiency decreases as $N$ grows, converging to zero for large $N$, showing weak scaling.


## Exercise 2.11

Let's do a specific example. Assume that a code has a setup that takes 1 second and a parallelizable section that takes 1000 seconds on one processor. What are the speedup and efficiency if the code is executed with 100 processors? What are they for 500 processors? Express your answer to at most two significant digits.

Answer: Let the total execution time on one processor be $T_1 = 1 + 1000 = 1001$ seconds. The sequential fraction of the code is $F_s = \frac{1}{1001}$, and the parallelizable fraction is $F_p = \frac{1000}{1001}$. To compute the speedup and efficiency for 100 processors, we use Amdahl’s Law: 

$$ T_P = T_1(F_s + F_p/P)$$
$$ T_{100} = 1001 \left( \frac{1}{1001} + \frac{1000}{1001 \times 100} \right) = 11$$

The speedup is:

$$ S_{100} = \frac{T_1}{T_{100}} = \frac{1001}{11} = 91$$

and the efficiency is:

$$ E_{100} = \frac{S_{100}}{p} = \frac{91}{100} = 0.91$$

For 500 processors, we similarly compute:

$$ T_{500} = 1001 \left( \frac{1}{1001} + \frac{1000}{1001 \times 500} \right) = 3$$

The speedup is:

$$ S_{500} = \frac{1001}{3} = 333.6667 $$

and the efficiency is:

$$ E_{500} = \frac{333.6667}{500} = 0.6673334$$

Using the above formulas, we find that for 100 processors, the speedup is approximately 91 and the efficiency is 0.91, while for 500 processors, the speedup is approximately 334 and the efficiency is 0.67.

## Exercise 2.12

Investigate the implications of Amdahl's law: if the number of processors $P$ increases, how does the parallel fraction of a code have to increase to maintain a fixed efficiency?

Answer: Consider the speedup and efficiency given by Amdahl’s law:  

$$ S_P = \frac{1}{F_s + \frac{F_p}{P}} $$  

$$ E_P = \frac{S_P}{P} = \frac{1}{P \left( F_s + \frac{F_p}{P} \right)} $$  

To maintain a fixed efficiency as $P$ increases, the denominator must remain constant. This requires either increasing $F_p$ or reducing $F_s$ by converting some of the sequential work into parallel work.  

Since $F_s$ may represent setup or initialization costs, minimizing it could help. For large $P$, $\frac{F_p}{P}$ becomes negligible, making $F_s$ the dominant term. Thus, to sustain efficiency, $F_p$ must scale with $P$ to counterbalance the effect of $F_s$.  

In short, as $P$ grows, efficiency declines unless $F_p$ increases. If $F_s$ stays constant, speedup is limited, and efficiency drops. To maintain efficiency, the parallel fraction must grow proportionally with $P$.  

## Exercise 2.17

Answer

## Exercise 2.19

Answer

## Exercise 4.1

Answer

## Exercise 4.2

Answer

## Exercise 4.13

Answer

## Exercise 4.14

Answer