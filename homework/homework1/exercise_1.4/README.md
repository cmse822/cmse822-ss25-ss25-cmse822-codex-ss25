## Exercise 1.4

The operation
```
for (i) {
  x[i+1] = a[i]*x[i] + b[i];
}
```
can not be handled by a pipeline because there is a dependency between input of one iteration of the operation and the output of the previous. However, you can transform the loop into one that is mathematically equivalent, and potentially more efficient to compute. Derive an expression that computes x[i+2] from x[i] without involving x[i+1]. This is known as recursive doubling. Assume you have plenty of temporary storage. You can now perform the calculation by
- Doing some preliminary calculations;
- computing x[i],x[i+2],x[i+4],..., and from these, • compute the missing terms x[i+1],x[i+3],....

Analyze the efficiency of this scheme by giving formulas for 𝑇0(𝑛) and 𝑇𝑠(𝑛). Can you think of an argument why the preliminary calculations may be of lesser importance in some circumstances?

## Answer
In the example code the processot can only start calculating `x[i+1]` once it is finished calculating `x[i]`, leaving pipeline slots unused while it does the calculation for `x[i]`.

A pipeline with 5 slots could be represented (in a very simplistic way) like this:
```
[a[1]*x[1]] -> [] -> [] -> [] -> []
[] -> [res + b[1]] -> [] -> [] -> []
[] -> [] -> [x[2] = res] -> [] -> []
[] -> [] -> [] -> [res] -> []
[] -> [] -> [] -> [] -> [res]
[] -> [] -> [] -> [] -> [] -> res
```

If we were to reformulate to calculate `x[i+2] = a[i+1]*(a[i]*x[i] + b[i]) + b[i+1]` the processor can divide the work and take advantage of the pipeline.

```
[a[1]*x[1]] -> []           -> []           -> []           -> []
[a[2]*x[2]] -> [res + b[1]] -> []           -> []           -> []
[]          -> [res + b[2]] -> [a[3] * res] -> []           -> []
[]          -> []           -> [a[4] * res] -> [res + b[3]] -> []
[]          -> []           -> []           -> [res + b[4]] -> [res1]
[]          -> []           -> []           -> []           -> [res2] -> x[3] = res1 
[]          -> []           -> []           -> []           -> []     -> x[4] = res2
```
This is true as to calculate `x[3]` we would only depend on `x[1]`. `x[4]` would only depend on `x[2]`.
As shown in the example, while calculating `x[4]` we would not need to wait for `x[3]` to be completed; therefore we can calculate `x[4]` and `x[3]` in "parallel" and take advantage of pipelining, while it is performing the operations for `x[4]` other pipeline slots could be filled to operate on `x[3]`.

Formulas for the efficiency would be:
- With `k` being the number of operations to complete one `a*x + b` operation
- With `l` as the number of stages in the pipeline:
- With `z` as a multiplicator of how many more operations need to be done

`T0(n) = n * k * l`

`T1(n) = (z * k) + z * (n + l - 1) * k`

> ![alt text](image.png)
> `k = 2 | l = 3 | z = 2`
> - T0(n) = Green line
> - T1(n) = Purple line 

* It is worth noting that for high values for z, recursive doubling does not make the computation more efficient, as it would require too much overhead for each calculation.
## Code
...