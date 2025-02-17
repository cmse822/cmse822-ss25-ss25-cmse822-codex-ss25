For this problem, assume the algorithm has a runtime of $n\log n$ using the best serial algorithm and a runtime of $(\log n)^2$ using a parallel algorithm.

In the weak scaling case where $n=p$, speedup is
$$\frac{T_1}{T_p} = \frac{n\log n}{(\log n)^2} = \frac{n}{\log n} = \frac{p}{\log p}$$

In the strong scaling case, $T_p = (\log n)^2 \cdot \frac{c}{p}$ for a constant $c$ since the time decreases linearly as we increase the processor count. Then, speedup is
$$\frac{T_1}{T_p} = \frac{n\log n}{(\log n)^2 \cdot \frac{c}{p}} = \frac{pn}{c\log n}$$