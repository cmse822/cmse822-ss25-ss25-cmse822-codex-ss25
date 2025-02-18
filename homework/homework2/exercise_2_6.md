## Exercise 2.6
 
Prove that $𝐸 = 1$ implies that all processors are active all the time. (Hint: suppose all processors finish their work in time $𝑇$, except for one processor in $𝑇^′ < 𝑇$. What is $𝑇_𝑝$ in this case? Explore the above relations.)

If we suppose that all but one processor finishes their work in time $T$ and the other finishing in time $T^{'}$ where $T^{'} \\leq T$, $T_p$ is the following:

$$T_p = ((p-1)*T + T')$$

Which gives,

$$S_p = \frac{T_1}{((p-1)*T + T')}$$

and,

$$E_p = \frac{T_1}{((p-1)*T + T')p}$$

In this case, for $E$ to equal $1$, the following conditions must be met:

* $T' = T$
* $T_1 = T_pp \\rightarrow ((p-1)*T + T)p \\rightarrow Tp^2$ (the ideal case)

Therefore,

$$E_p = \frac{Tp^2}{((p-1)*T + T)p}$$

$$E_p = \frac{Tp^2}{(p*T)p}$$

$$E_p = \frac{Tp^2}{(p^2*T)}$$

$$E_p = \frac{Tp^2}{Tp^2}$$

$$E_p = 1$$

From this, we see that for $E$ to be $1$, all of the processors must have the same execution time, which is the ideal case as $T_1$ represents ‘the best time to solve the problem on a single processor’. This implies that the processors must all be running and have the same execution time to reach maximum efficiency.
