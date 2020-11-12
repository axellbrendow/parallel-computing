#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

  if (rank == 0) {
    // preencher o buffer com N valores inteiros aleatórios
    for (size_t i = 0; i < N; i++) {
      buffer[i] = rand();
      printf("buffer[%d] = %d\n", i, buffer[i]);
    }
  } 

  // distribuir o vetor para todos os outros processos
  MPI_Bcast(
      buffer,
      N,
      MPI_INT,
      0,
      MPI_COMM_WORLD);

  // processar o maior dos valores dentro do seu intervalo
  int greatest = INT_MIN;

  for (int i = rank; i < N; i += numProcs)
    greatest = buffer[i] > greatest ? buffer[i] : greatest;

  // reduzir os maiores no maior, enviando o resultado para o processo com rank = 0
  MPI_Reduce(
    &greatest,
    &maior_final,
    1,
    MPI_INT,
    MPI_MAX,
    0,
    MPI_COMM_WORLD);

  if (rank == 0) {
    printf("maior_final = %d\n", maior_final);
  }
  
  MPI_Finalize();
}
