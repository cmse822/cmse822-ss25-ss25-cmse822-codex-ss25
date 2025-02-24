# Exercise 2.12

## Question
Investigate the implications of Amdahl’s law: if the number of processors 𝑃 increases, how does the parallel fraction of a code have to increase to maintain a fixed efficiency?

## Answer
First, we can derive equations for the Speedup and Efficiency, given Amidahl's law. <br>
$S_p = \frac{T_1}{T_p}, T_p = T_1(F_s + \frac{F_p}{p})$ <br>
$S_p = T_1[T_1(F_s + \frac{F_p}{p})]^{-1}$ <br>
$S_p = (F_s + \frac{F_p}{p})^{-1}$

$E = \frac{S_p}{p} = (pF_s + F_p)^{-1} = (p(1-F_p) +F_p)^{-1}$ <br>

Now, we will want to find an equation for $F_p$ that satisfies the following; <br>
$\frac{d}{dp}E = 0$ <br>

In order for this to be true, we must find that; <br>
$\frac{d}{dp}(p(1-F_p) + F_p) = 0$ <br>
$F_p'(1-p) - F_p + 1 = 0$ <br>

Rearranging, we can find; <br>
$F_p = F_p'(1-p) + 1$ <br>
$F_p = \frac{dF_p}{dp}(1-p)+1$ <br>
$-\frac{F_p - 1}{p-1} = \frac{dF_p}{dp}$ <br>
$-\frac{dF_p}{F_p-1} = \frac{dp}{p-1}$ <br>

We can then integrate; <br>
$-\int\frac{1}{F_p-1}dF_p = \int\frac{1}{p-1}dp$ <br>
$-\ln(F_p-1) + C = \ln(p-1) + C$ <br>
$\ln(F_p-1) = C - \ln(p-1)$ <br>
$F_p-1 = \frac{e^C}{p-1}$ <br>
$F_p = \frac{C}{p-1}+1$ <br>

We find the final relationship;
$$F_p = -\frac{C}{p-1}+1, \space F_s = \frac{C}{p-1}$$
Where C is some positive constant that controls the target efficiency. This relationship is only valid when $p \ge C + 1$ and $p > 1$. <br>
Under this relationship, the Efficiency of this system remains constant as the number of processors increases. The general behavior indicates that, as the number of processors increases, the parallel fraction needs to asymptotically approach 100% in order for the efficiency to remain constant. 

Along the same lines, the operating term $\frac{C}{p-1}$ represents the influence of the Sequential Fraction, whose effect is magnified as the number of processors increases, and must asymptotically approach 0% for the efficiency to remain constant.