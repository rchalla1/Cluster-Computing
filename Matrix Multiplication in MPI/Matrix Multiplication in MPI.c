#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define PREC 1e-6 

double A[1500][900];
double B[900][1200];
double C[1500][1200];

void initMatrix(int ib, int ic){
  int i ,j;

  for(i = ib ; i < ic ; i++)
    for(j = 0 ; j < 900 ; j++)
      A[i][j] = (i+1)*(j+1);

  for(i = 0; i < 900; i++)
    for(j = 0 ; j < 1200 ; j++)
      B[i][j] = 1/((double) (i+1)* (double)(j+1));

  for(i = ib ; i < ic; i++)
    for(j = 0 ; j < 1200 ; j++)
      C[i][j] = 0.;

}
void mXm(int ib, int ic){
  int i,j,k;

  for(i = ib ; i < ic ; i++)
    for(j = 0 ; j < 1200 ; j++)
      for(k = 0 ; k < 900 ; k++)
	C[i][j] += A[i][k] * B[k][j];
}

void ckCo(int ib, int ic){
  int i,j;
  double testC;

  for(i = ib ; i < ic ; i++)
    for(j = 0 ; j < 1200 ; j++){
      testC=900*(double)(i+1)/(double) (j+1); // matrix Comparision
      if( fabs(testC-C[i][j]) > PREC ){
	printf("Both Matrixes are not equal\n");
	MPI_Finalize();
	exit(-1);
      }
    }
}

int main(int argc, char *argv[]){

  int myID, Processes, ib, ic;
  int i,j,k;
  double starttime, endtime;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&myID); 
  MPI_Comm_size(MPI_COMM_WORLD,&Processes);

  ib = myID*(1500/Processes);
  ic = (myID == Processes-1) ? (myID+1)*(1500/Processes) + 1500%Processes : (myID+1)*(1500/Processes); 

  initMatrix(ib,ic);

  starttime = MPI_Wtime();
  mXm(ib,ic);
  endtime = MPI_Wtime();
  if(myID == 0) printf(" The number of Processes= %d for calculating matrix multiplication and it's time= %f secs\n",Processes,endtime-starttime);
  ckCo(ib,ic);

  MPI_Finalize();
  return 0;
}
