#include <stdio.h>
#include <omp.h>

int main()
{
    int i;

// 'ordered' forces that certain events within the loop happen in a predicted order
#pragma omp parallel for ordered num_threads(2)
    for (i = 1; i <= 3; i++)
    {
        int tid = omp_get_thread_num(); // get thread identifier
#pragma omp ordered
        printf("[PRINT1] T%d = %d \n", tid, i);
        printf("[PRINT2] T%d = %d \n", tid, i);
    }
}
