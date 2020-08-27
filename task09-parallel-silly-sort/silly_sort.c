#include <stdio.h>
#include <stdlib.h>

/*
SEQUENTIAL PERF STATS:

 Performance counter stats for './silly_sort.exe':

       2161,394750      task-clock (msec)         #    0,993 CPUs utilized          
               134      context-switches          #    0,062 K/sec                  
                 1      cpu-migrations            #    0,000 K/sec                  
               163      page-faults               #    0,075 K/sec                  
     7.165.059.522      cycles                    #    3,315 GHz                      (49,78%)
    19.640.049.578      instructions              #    2,74  insn per cycle           (62,28%)
     1.796.169.662      branches                  #  831,023 M/sec                    (62,49%)
           121.151      branch-misses             #    0,01% of all branches          (62,85%)
     9.536.134.835      L1-dcache-loads           # 4412,028 M/sec                    (62,69%)
        56.942.537      L1-dcache-load-misses     #    0,60% of all L1-dcache hits    (62,70%)
           331.781      LLC-loads                 #    0,154 M/sec                    (49,83%)
            67.009      LLC-load-misses           #   20,20% of all LL-cache hits     (49,66%)

       2,175964001 seconds time elapsed


PARALLEL PERF STATS schedule(static, 100):

 Performance counter stats for './silly_sort.exe':

       2220,308851      task-clock (msec)         #    1,999 CPUs utilized          
                 7      context-switches          #    0,003 K/sec                  
                 0      cpu-migrations            #    0,000 K/sec                  
               185      page-faults               #    0,083 K/sec                  
     7.158.248.322      cycles                    #    3,224 GHz                      (49,56%)
    21.288.725.378      instructions              #    2,97  insn per cycle           (62,17%)
     3.167.229.133      branches                  # 1426,481 M/sec                    (62,17%)
           112.652      branch-misses             #    0,00% of all branches          (62,43%)
     9.403.888.610      L1-dcache-loads           # 4235,397 M/sec                    (62,79%)
        56.250.854      L1-dcache-load-misses     #    0,60% of all L1-dcache hits    (63,05%)
           426.481      LLC-loads                 #    0,192 M/sec                    (50,18%)
            49.346      LLC-load-misses           #   11,57% of all LL-cache hits     (49,83%)

       1,110849333 seconds time elapsed


PARALLEL PERF STATS WITHOUT SCHEDULE:

 Performance counter stats for './silly_sort.exe':

       2615,475821      task-clock (msec)         #    1,917 CPUs utilized          
                 6      context-switches          #    0,002 K/sec                  
                 0      cpu-migrations            #    0,000 K/sec                  
               183      page-faults               #    0,070 K/sec                  
     7.121.225.880      cycles                    #    2,723 GHz                      (49,78%)
    21.183.372.828      instructions              #    2,97  insn per cycle           (62,37%)
     3.203.374.395      branches                  # 1224,777 M/sec                    (62,49%)
           121.011      branch-misses             #    0,00% of all branches          (62,64%)
     9.582.222.750      L1-dcache-loads           # 3663,663 M/sec                    (62,80%)
        56.559.637      L1-dcache-load-misses     #    0,59% of all L1-dcache hits    (62,64%)
           325.689      LLC-loads                 #    0,125 M/sec                    (49,90%)
            62.292      LLC-load-misses           #   19,13% of all LL-cache hits     (49,74%)

       1,364121956 seconds time elapsed
*/

int main() 
{
   int i, j, n = 30000; 

   // Allocate input, output and position arrays
   int *in = (int*) calloc(n, sizeof(int));
   int *pos = (int*) calloc(n, sizeof(int));   
   int *out = (int*) calloc(n, sizeof(int));   

   // Initialize input array in the reverse order
   for(i=0; i < n; i++)
      in[i] = n-i;  

   // Print input array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",in[i]);
    
   // Silly sort (you have to make this code parallel)
   #pragma omp parallel for num_threads(2) private(j)
   for(i=0; i < n; i++) 
      for(j=0; j < n; j++)
	     if(in[i] > in[j]) 
            pos[i]++;	

   // Move elements to final position
   for(i=0; i < n; i++)
      out[pos[i]] = in[i];
   
   // print output array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",out[i]);

   // Check if answer is correct
   for(i=0; i < n; i++)
      if(i+1 != out[i]) 
      {
         printf("test failed\n");
         exit(0);
      }

   printf("test passed\n"); 
}  
