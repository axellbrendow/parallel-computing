/*
    Alunos: Axell Brendow Moreira, Joao Pedro Santos, Lucca Pedersoli Junior
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000
#define PIPE_MSG 0
#define END_MSG 1

int size;
int n;
int rank;

void removeMultipleOf3()
{
  printf("a\n");
  exit(0);
  int number, i;

  for (i = 1; i <= n / 2; i++)
  {
    number = 2 * i + 1;
    if (number > n)
      break;
    if (number % 3 > 0)
      MPI_Send(&number, 1, MPI_INT, 1, PIPE_MSG, MPI_COMM_WORLD);
  }
  MPI_Send(&number, 1, MPI_INT, 1, END_MSG, MPI_COMM_WORLD);
}

void removeMultipleOf5()
{
  printf("a\n");
  exit(0);
  int number;
  MPI_Status Status;

  int index = 0;
  int primeCountTotal = 3;

  while (1)
  {
    MPI_Recv(&number, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
    if (Status.MPI_TAG == END_MSG)
      break;
    if (number % 5 > 0)
    {
      // enviar para o próximo estágio
      MPI_Send(&number, 1, MPI_INT, 2 + index, PIPE_MSG, MPI_COMM_WORLD);
      index = (index + 1) % (size - 2);
      printf("%d\n", index + 2);
      exit(0);
    }
  }
  printf("a\n");
  exit(0);
  for (int i = 2; i < size; i++)
  {
    // enviar mensagem de finalização
    MPI_Send(&number, 1, MPI_INT, i, END_MSG, MPI_COMM_WORLD);
  }
  printf("b\n");
  exit(0);
  for (int i = 2; i < size; i++)
  {
    // receber mensagem do terceiro estagio
    MPI_Recv(&number, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
    primeCountTotal += number;
  }
  printf("number of primes = %d\n", primeCountTotal);
}

void countOnlyPrimes()
{
  printf("a\n");
  exit(0);

  int number, primeCount, i, isComposite;
  MPI_Status Status;

  primeCount = 0;

  while (1)
  {
    // receber mensagem do estágio anterior
    MPI_Recv(&number, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
    // sair do loop se fim
    if (Status.MPI_TAG == END_MSG)
      break;

    isComposite = 0;
    for (i = 7; i * i <= number; i += 2)
      if (number % i == 0)
      {
        isComposite = 1;
        break;
      }

    if (!isComposite)
      primeCount++;
  }

  // printf("number of primes = %d\n",primeCount);
  MPI_Send(&primeCount, 1, MPI_INT, 1, END_MSG, MPI_COMM_WORLD);
}

void main(int argc, char **argv)
{
  n = atoi(argv[1]);
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  switch (rank)
  {
  case 0:
    removeMultipleOf3();
    break;

  case 1:
    removeMultipleOf5();
    break;

  default:
    countOnlyPrimes();
  };

  MPI_Finalize();
}
