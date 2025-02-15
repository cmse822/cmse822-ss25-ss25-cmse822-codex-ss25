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
      

Casse 1: For ```100``` processors
From equation (2), we can write that; 
      ```T_p = 1 + 1000 = 1001``` seconds 
But from equation (1), we can write that;
      ```F_p = (1-F_s)```                    (3)
Substituting ```T_P``` and equation (3) into equation (2) gives us;
    ```1001 = T_1(F_s + (1-F_s)/100)```       (4)
But we are told that the set up takes 1 second meaning that  ```𝑇_1F_s = 1.``` This implies that  ```𝑇_1 = 1/F_s.```
Substituting for ```T_1``` in equation (4) gives us;
    ```1001 =  1/F_s (F_s + (1-F_s)/100)```    (5)
    ```1001F_s = (F_s + (1-F_s)/100)``` 
    ```100100F_s = 100F_s + 1 - F_s```
    ```100001F_s = 1```
    ```F_s = 0.0000.9999```
Therefore the speedup, ```S_p = T_1/p = (1/F_s)/p = 100001/100 = 1000.01```
The Efficiency, ```E_p = S_p/p = 1000.01/100 = 10.0001```

Case 2: For ```500``` processors
The speedup, ```S_p = T_1/p = 100001/500 = 200.002```
The Efficiency, ```E_p = S_p/p = 200.02/500 = 0.400004```


    
        


      
