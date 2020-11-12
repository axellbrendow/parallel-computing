#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10
#define MAX 4
#define NUMBER 3

void main(int argc, char* argv[]) {
  int p, rank, maior_parcial, maior_final, numProcs;
  int buffer[N];
  MPI_Status status;
  srand(time(NULL));

  MPI_Init(&argc, &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  int numElementsPerProcess = (N / numProcs) + 1;
  int nums[numElementsPerProcess];

  if (rank == 0) {
    // preencher o buffer com N valores inteiros aleatórios
    for (size_t i = 0; i < N; i++) buffer[i] = rand();
  } 

  // distribuir o vetor para todos os outros processos
  MPI_Scatter(
      buffer,
      numElementsPerProcess,
      MPI_INT,
      &nums,
      numElementsPerProcess,
      MPI_INT,
      0,
      MPI_COMM_WORLD);

  for (int i = 0; i < numElementsPerProcess; i++)
    printf("rank = %d, value[i] = %d\n", rank, nums[i]);

  // processar o maior dos valores dentro do seu intervalo

  // reduzir os maiores no maior, enviando o resultado para o processo com rank = 0

  if (rank == 0) {
    // imprimir maior
  }
  
  MPI_Finalize();
}
