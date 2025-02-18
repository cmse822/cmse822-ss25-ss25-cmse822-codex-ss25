# Exercise 2.11

## Question
Let’s do a specific example. Assume that a code has a setup that takes 1 second and a parallelizable section that takes 1000 seconds on one processor. What are the speedup and efficiency if the code is executed with 100 processors? What are they for 500 processors? Express your answer to at most two significant digits.

## Answer
First, we can derive equations for the Speedup and Efficiency, given Amidahl's law. <br>
$S_p = \frac{T_1}{T_p}, T_p = T_1(F_s + \frac{F_p}{p})$ <br>
$S_p = T_1[T_1(F_s + \frac{F_p}{p})]^{-1}$ <br>
$S_p = (F_s + \frac{F_p}{p})^{-1}$

$E = \frac{S_p}{p}$ <br>

From here, we can then calculate both, given the provided values; <br>
$F_s = 1 / 1001, F_p = 1000/1001$ <br>

$F_s(100) = (\frac{1}{1001} + \frac{1000}{1001 * 100})^{-1} = 91$ <br>
$E(100) = 91 / 100 = .91$ <br>

$F_s(500) = (\frac{1}{1001} + \frac{1000}{1001 * 500})^{-1} = 333.67$ <br>
$E(500) = 333.67 / 500 = .6673$ <br>

We found;
$$S(100) = 91, E(100) = .91$$
$$S(500) = 333.67, E(500) = .6673$$