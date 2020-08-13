/*
Adaptado de: https://ideone.com/JU5CfV
e https://github.com/stbrumme/eratosthenes

---

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

const int blockSize = 128 * 1024;
const int n = 100000000;
const bool useOpenMP = true;

// Processar os numeros impares de blocos especificos
int eratosthenesOddSingleBlock(const int from, const int to)
{
   const int memorySize = (to - from + 1) / 2;

   // initializa
   char *isPrime = malloc(memorySize * sizeof(char));

   for (int i = 0; i < memorySize; i++)
      isPrime[i] = 1;

   for (int i = 3; i * i <= to; i += 2)
   {
      // pular multiplos de 3
      if (i >= 3 * 3 && i % 3 == 0)
         continue;
      // pular multiplos de 5
      if (i >= 5 * 5 && i % 5 == 0)
         continue;
      // pular multiplos de 7
      if (i >= 7 * 7 && i % 7 == 0)
         continue;
      // pular multiplos de 11
      if (i >= 11 * 11 && i % 11 == 0)
         continue;
      // pular multiplos de 13
      if (i >= 13 * 13 && i % 13 == 0)
         continue;

      // pula anteriores a metade
      int minJ = ((from + i - 1) / i) * i;
      if (minJ < i * i)
         minJ = i * i;

      // valor inicial tem de ser impar
      if ((minJ & 1) == 0)
         minJ += i;

      // encontra todos os nao primos impares
      for (int j = minJ; j <= to; j += 2 * i)
      {
         int index = j - from;
         isPrime[index / 2] = 0;
      }
   }

   // conta os primos nesse bloco
   int found = 0;
   for (int i = 0; i < memorySize; i++)
      found += isPrime[i];
   // 2 nao e' impar
   if (from <= 2)
      found++;

   free(isPrime);
   return found;
}

// Fragmentar o crivo em blocos para facilitar o paralelismo
int eratosthenesBlockwise(int fim)
{
   // habilitar openmp
   omp_set_num_threads(useOpenMP ? omp_get_num_procs() : 1);

   double found = 0;

   #pragma omp parallel for reduction(+ : found)
   for (int from = 2; from <= fim; from += blockSize)
   {
      int to = from + blockSize;

      if (to > fim)
         to = fim;

      found += eratosthenesOddSingleBlock(from, to);
   }

   return found;
}

int sieveOfEratosthenes(int n)
{
   // Create a boolean array "prime[0..n]" and initialize
   // all entries it as true. A value in prime[i] will
   // finally be false if i is Not a prime, else true.
   int primes = 0;
   bool *prime = (bool *)malloc((n + 1) * sizeof(bool));
   int sqrt_n = sqrt(n);

   memset(prime, true, (n + 1) * sizeof(bool));

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
   for (int p = 2; p <= n; p++)
      if (prime[p])
         primes++;

   return (primes);
}

int main()
{
   clock_t tStart = clock();
   int eratosthenes = sieveOfEratosthenes(n);
   printf("Erastothenes Sequencial: %d / Tempo: %.2fs\n", eratosthenes, (double)(clock() - tStart) / CLOCKS_PER_SEC);

   tStart = clock();
   int parallelratosthenes = eratosthenesBlockwise(n);
   printf("Erastothenes Paralelo: %d / Tempo: %.2fs\n", parallelratosthenes, (double)(clock() - tStart) / CLOCKS_PER_SEC);
   return 0;
}
