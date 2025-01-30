Programming language: C versus Fortran. If your loop describes the (𝑖, 𝑗)indices of a two-dimensional
array, it is often best to let the 𝑖-index be in the inner loop for Fortran, and the 𝑗-index inner for C.
We are required to come you come up with at least two reasons why this is possibly better for performance.

1. In Fortarn, the arrays are stored in a column-major order which means that the elemnts are stored next to
   each other in columns without leaving any gaps in the first memmory (this means that they are accessed
   sequentially in this format). It is best to let i-index be in the inner loop becuase this suits the odrer
   hence improving the cache performance(avoids misses and breaks) and memory usage.

 3. In C, the arrays  in the row-major order which means that elements are stored and accesssed essentially in
    rows in the first row in the first memory then the the second row comes next. Similary like in fortran, this
    improves the cache performance (avoids misses and breaks) and memory usage.

# Reference
Sterling, Thomas, and Matthew Anderson. *High Performance Computing: Modern Systems and Practices*. 1st ed., Morgan Kaufmann, 2008.
