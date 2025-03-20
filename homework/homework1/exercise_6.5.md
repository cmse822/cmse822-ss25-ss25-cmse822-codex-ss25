### Question

```
bs = ... /* the blocksize */ 
for (b=0; b<100000/bs; b++)
  for (n=0; n<10; n++)
    for (i=b*bs; i<(b+1)*bs; i++)
      ... = ...x[i] ...
```
Analyze this example. When is x brought into cache, when is it reused, and when is it flushed? What is the required cache size in this example? Rewrite this example, using a constant `#define L1SIZE 65536`

### Answer
In this case x is brought into cache on line 5, as it is used on the calculation; each address of x is then added to the cache. As it uses _Loop Tiling_ it does the operations in blocks, each block with size `bs`.

Because it uses _Loop Tiling_ x is reused 10 times, as long as it fits on cache. In this case the best approach would be to keep the blocksize as the same size as the L1 cache, taking most advantage of cache look up.

X is then flushed when there is no more space in cache, and ideally this only happens when we move to the next block and do not need the 'old' values of x anymore. This would happen when the cache has a size of `bs`.

To take advantage of the block size the following code could be used:

```
#define L1SIZE 65536
bs = L1SIZE / sizeof(double) /* assuming x is of type double*/
for (b=0; b<100000/bs; b++)
  for (n=0; n<10; n++)
    for (i=b*bs; i<(b+1)*bs; i++)
      ... = ...x[i] ...
```

