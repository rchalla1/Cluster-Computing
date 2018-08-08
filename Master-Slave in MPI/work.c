#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define MASTER 0
#define WORK_P1 1
#define WORK_P2 2

const int MAX = 40000;
static int n = 0;
int main(int argc, char **argv)
{
  int rank, size;
  double esum, osum, oddavg, evenavg;
  MPI_Status status;
  double arr[MAX];
  int i;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == MASTER)
  {
    //printf("Initial array: ");
    for (i = 0; i < MAX; i++)
    {
      arr[i] = n++;
      //printf("%d=%f\n ", i,arr[i]);
    }
    //MPI_Send(&(arr[i]),1,MPI_DOUBLE,1,543, MPI_COMM_WORLD);
    //printf("\n");
    for (i = 0; i < MAX; i++)
    {
      if (i % 2 == 1)
      {
        //printf("odd%d=%f\n ", i,arr[i]);
        osum += arr[i];
      }
      else
      {
        esum += arr[i];
        //printf("even%d=%f\n ", i,arr[i]);
      }
      //MPI_Send(&(arr[i]),1,MPI_DOUBLE,1,543, MPI_COMM_WORLD);
    }
    MPI_Send(&osum, 1, MPI_DOUBLE, 1, 999, MPI_COMM_WORLD);
    MPI_Send(&esum, 1, MPI_DOUBLE, 2, 999, MPI_COMM_WORLD);
  }
  if (rank == WORK_P1)
  {
    //MPI_Recv(&(arr[i]), 1, MPI_DOUBLE, 0, 543, MPI_COMM_WORLD,&status);
    MPI_Recv(&osum, 1, MPI_DOUBLE, 0, 999, MPI_COMM_WORLD, &status);
    oddavg = osum / 20000;
    printf("The Average of Odd Index Elements=%f for Processor %d\n", oddavg, rank);
    //printf("\n");
  }
  else if (rank == WORK_P2)
  {
    //MPI_Recv(&(arr[i]), 1, MPI_DOUBLE, 0, 543, MPI_COMM_WORLD,&status);
    MPI_Recv(&esum, 1, MPI_DOUBLE, 0, 999, MPI_COMM_WORLD, &status);
    evenavg = esum / 20000;
    printf("The Average of Even Index Elements =%f for Processor %d\n", evenavg, rank);
    //printf("\n");
  }
  MPI_Finalize();
  return 0;
}