#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int tid, nthreads, i, j, k;
  double a[1500][900],
      b[900][1200],
      c[1500][1200],
      d[1500][1200];

  double dif, start, end;

#pragma omp parallel shared(a, b, c, d, nthreads) private(tid, i, j, k)
  {

    tid = omp_get_thread_num();
    if (tid == 0)
    {
      nthreads = omp_get_num_threads();
      // printf("Starting matrix multiple example with %d threads\n",nthreads);
      // printf("Initializing matrices...\n");
    }
    start = omp_get_wtime();

#pragma omp for schedule(static)
    for (i = 0; i < 1500; i++)
      for (j = 0; j < 900; j++)
        a[i][j] = (i + 1) * (j + 1);
#pragma omp for schedule(static)
    for (i = 0; i < 900; i++)
      for (j = 0; j < 1200; j++)
        b[i][j] = 1 / ((double)(i + 1) * (double)(j + 1));
#pragma omp for schedule(static)
    for (i = 0; i < 1500; i++)
      for (j = 0; j < 1200; j++)
        c[i][j] = 0;

#pragma omp for schedule(static) //Matrix Multiplication
    for (i = 0; i < 1500; i++)
    {
      for (j = 0; j < 1200; j++)
      {
        for (k = 0; k < 900; k++)
          c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  end = omp_get_wtime();
  dif = end - start;
  printf("Parallelization of Matrix Multiplication in static Scheduling %f sec. time for %d Thread(s)\n", dif, nthreads);
}
