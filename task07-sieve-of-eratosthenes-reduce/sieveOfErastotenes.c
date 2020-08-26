/*
Adaptado de: https://ideone.com/JU5CfV
e https://github.com/stbrumme/eratosthenes

---

//

Medição do tempo usando clock

Erastothenes Sequencial: 5761455 / Tempo: 2.04s
Erastothenes Paralelo: 5761455 / Tempo: 1.07s

Speed up: 1.9

---

Medição do tempo usando comando time

Erastothenes Sequencial: 1,95s user 0,03s system 99% cpu 1,977 total
Erastothenes Paralelo: 1,53s user 0,00s system 514% cpu 0,299 total

Speed up: 1.2
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

   #pragma omp parallel for schedule(dynamic,100)
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
