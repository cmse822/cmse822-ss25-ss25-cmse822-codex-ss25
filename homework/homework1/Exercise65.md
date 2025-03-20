We are required to analyze example on page 275. When is x brought into cache, when is it reused, and when
is it flushed? What is the required cache size in this example? Rewrite this example, using
a constant
#define L1SIZE 65536

# Comparison: Without Tiling vs With Tiling

## Without Tiling:
  - **Cache:** X is brought into cache the very first time it is used over the $100000$ iterations. 
  - **Reused:** X is reused on the second iteration of the outerloop
  - **Flushed:** When X is too large to fit the cache
---

## With Tiling:
  - **Cache:** X is brought into cache the very first time it is used. In this case the first small block, `bs`.
  - **Reused:** X is rused on the second iteration of the middle loop
  - **Flushed:** Everytime we have a new block, what was in the previous cache is replaced by the new one. 


# The required cache size in this example.

Let  `bs` be the cache size:

$$
bs = \frac{L1SIZE}{\text{sizeof(double)}}
$$

$$
bs = \frac{65536}{8} = 8192
$$

Therefore, the required cache size is $8192$.


# Rewriting this example, using a constant #define L1SIZE 65536
_Disclaimer: This code was generated with the assistance of AI_

```c
#include <stdio.h>

#define L1SIZE 65536  // 
#define bs (L1SIZE / sizeof(double))  //

double x[100000];  //

void loop_tiling() {
    int b, i, n;
    int nblocks = 100000 / bs;
    int remainder = 100000 % bs;  // 

    for (b = 0; b < nblocks; b++) {
        for (n = 0; n < 10; n++) {  
            for (i = b * bs; i < (b + 1) * bs; i++) {  
                x[i] = x[i] * 2.0;  // 
            }
        }
    }

    for (n = 0; n < 10; n++) {
        for (i = nblocks * bs; i < 100000; i++) {
            x[i] = x[i] * 2.0;
        }
    }
}

int main() {
    loop_tiling();
    return 0;
}
