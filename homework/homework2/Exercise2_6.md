# Exercise 2.6

We are required to prove that ```𝐸 = 1``` implies that all processors are active all the time.
(Hint: suppose all processors finish their work in time ```𝑇,``` except for one processor in
```𝑇' < 𝑇``` . What is ```𝑇_𝑝``` in this case? Explore the above relations.)

  From section 2.2.1.1 (Speedup and efficiency) page 62 Textbook of the lecture notes, 
    Let 
        ```p``` be the processors, 
        ```T_1``` the execution time on a single processor and 
        ```T_p``` the time on the ```p``` processors
        the speed up ```sp = T_1/T_P```
    in the ideal case, ```T_p = T_1/p```  but in practice, we dont expect to obtain that, 
    so ```sp ≤ p.``` To measure how far we are from the ideal speedup, we introduce the
    efficiency ```E_p = S_p/p.``` Clearly, ```0  ≤ E_p ≤ 1.```

  From the given information, this implies that;
          ```E_p = T_1/pT_p```
  Suppose that ```E=E_p=1```, then this implies that ```T_P = T_1/p``` which is exactly the parallel execution time in the ideal case 
  as we saw in the definition in the textbook. This means that all the processors are fully utilized at that time. 

  However, we know that in practice, we cannot attain that, so ```S_p ≤ p.```  we introduce the
    efficiency ```E_p = S_p/p.``` Clearly, ```0  ≤ E_p ≤ 1.```. This implies that ```0  ≤ S_p/p ≤ 1 ``` and hence ```0  ≤ T_1/T ≤ 1.``` This 
    contradicts the statement that ```𝐸 = 1``` implies that all processors are active all the time. This therefore means that if a processor were idle
    at that time, then the efficiency ```E_p``` would be less less than one. 

  This completes the proof. 

    



