#include <stdio.h>

int main()
{
#pragma omp parallel num_threads(2)
   for (int i = 1; i <= 3; i++)
      printf("%d ", i);

   printf("\n");
}
