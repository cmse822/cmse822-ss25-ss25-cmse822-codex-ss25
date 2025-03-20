# Exercise 2.11
Let’s do a specific example. Assume that a code has a setup that takes 1 second and a parallelizable section
that takes ```1000``` seconds on one processor. What are the speedup and efficiency if the code is executed with 
```100``` processors? What are they for ```500``` processors? Express your answer to at most two significant digits.


From section 2.2.3 (Amdahl’s law) page 65 Textbook of the Lecture notes, we know that, 
      ```𝐹_𝑝 + 𝐹_𝑠 = 1.```                   (1)
where;
      ```𝐹_𝑠``` is the sequential fraction of a code and 
      ```𝐹_𝑝``` is the parallel fraction (or more strictly: the ‘parallelizable’ fraction) of a code
      
The parallel execution time ```𝑇_𝑝``` on ```𝑝``` processors is given by;
      ```𝑇_𝑃 = 𝑇_1 (𝐹_𝑠 + 𝐹_𝑝/𝑃 ).```        (2) 
where;
      ```𝑇_1F_s``` is the part that is sequential and 
      ```T_1F_p/p``` is the  the part that can be parallelized
      

Case 1: For ```100``` processors
Using equation (1), We know that we can write that; 
      ```T_1 = 1 + 1000 = 1001 seconds``` 
and
      ```𝑇_1F_s= 1``` and this implies that;
      ```F_s =1/T_1 =1/1001```
But from equation (1), we can write that;
      ```F_p = (1-F_s)
            = 1-1/1001
            = 1000/1001
            ```                    (3)
Substituting for ```F_p``` into equation (2) gives us;
    ```T_p = 1001(1/1001 + (1000/1001)/100)
          = 1001(1/1001 + (1000/100100)
          = 11``` 

Therefore the speedup, ```S_p = T_1/T_p = 1001/11 = 91```
The Efficiency, ```E_p = S_p/p = 91/100 = 0.91```

Case 2: For ```500``` processors
```T_p = 1001(1/1001 + (1000/1001)/500) = 3``` 
The speedup, ```S_p = T_1/T_p = 1001/3 = 333.67```
The Efficiency, ```E_p = S_p/p = (1001/3)/500 = 0.67```


    
        


      
