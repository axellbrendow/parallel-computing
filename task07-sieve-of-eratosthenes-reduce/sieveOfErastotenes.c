/*
Adaptado de: https://ideone.com/JU5CfV

---

Speed de 1,6

Sequencial

real    0m4.055s
user    0m3.981s
sys     0m0.068s

Paralelo static 100

real    0m2.934s
user    0m9.502s
sys     0m0.088s

Paralelo dynamic 100

real    0m2.511s
user    0m9.518s
sys     0m0.092s

Paralelo guided 100

real    0m3.670s
user    0m7.022s
sys     0m0.088s

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int sieveOfEratosthenes(int n)
{
   // Create a boolean array "prime[0..n]" and initialize
   // all entries it as true. A value in prime[i] will
   // finally be false if i is Not a prime, else true.
   int primes = 0;
   bool *prime = (bool *)malloc((n + 1) * sizeof(bool));
   int sqrt_n = sqrt(n);

   memset(prime, true, (n + 1) * sizeof(bool));

   #pragma omp parallel for schedule(static,100)
   for (int p = 2; p <= sqrt_n; p++)
   {
      // If prime[p] is not changed, then it is a prime
      if (prime[p] == true)
      {
         // Update all multiples of p
         for (int i = p * 2; i <= n; i += p)
            prime[i] = false;
      }
   }

   // count prime numbers
   #pragma omp parallel for reduction(+:primes)
   for (int p = 2; p <= n; p++)
      if (prime[p])
         primes++;

   return (primes);
}

int main()
{
   int n = 100000000;
   printf("%d\n", sieveOfEratosthenes(n));
   return 0;
}
