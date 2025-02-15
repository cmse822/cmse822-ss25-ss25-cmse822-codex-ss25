## Exercise 6.2
*While the strategy just sketched will demonstrate the existence of cache sizes, it will not report the maximal bandwidth that the cache supports. What is the problem and how would you fix it?*

### The Problem
To measure our maximum bandwidth, we're measuring the time it takes to complete calculations for varying dataset sizes to find at what sizes the time taken increases sharply. However, the time taken depends on not just bandwidth but also latency. To avoid memory prefetching, the current strategy randomly accesses data from memory. This results in high latency caused by cache misses, which would cause us to underestimate maximum bandwidth.

### The Solution
We want the measurement to be dominated by bandwidth costs, not latency. To ensure this, we could instead use a strided access pattern with strides longer than the cache lines but shorter than the cache size. This will result in much less latency when compared to a random access pattern, but it should still ensure that memory prefetching is avoided. We could also introduce more operations on the data rather than just loading and setting it. We can also improve this further by adding operations performed on the data beyond simply loading and/or setting values in an array. This would keep the data in cache for longer, ensuring the bandwidth is what's truly being measured.