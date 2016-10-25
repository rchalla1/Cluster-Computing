#include <mpi.h>
#include <stdio.h>

main(int argc, char* argv[]) {
  int rank,size;
  MPI_Status status;
  MPI_Request request;
  int done,myfound,inrange,nvalues;
  int b[40000];
  int i,j,temp;
  FILE *infile;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  myfound=0;
  if (rank==0) {
    infile=fopen("data.txt","r");
    for(i=0;i<40000;++i) {
      fscanf(infile,"%d",&b[i]);
    }
  }
  MPI_Bcast(b,40000,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Irecv(&temp,1,MPI_INT,MPI_ANY_SOURCE,86,MPI_COMM_WORLD,&request);
  MPI_Test(&request,&done,&status);
  nvalues=40000/size;
  i=rank*nvalues;
  inrange=(i<=((rank+1)*nvalues-1) && i>=rank*nvalues);
  while(!done && inrange) {
      if (b[i]==11) {
        temp=123;
        for(j=0;j<size;++j) 
		{
          MPI_Send(&temp,1,MPI_INT,j,86,MPI_COMM_WORLD);
        }
        printf("P:%d found element 11 at Array index %d\n",rank,i);
        myfound=1;
      }
      MPI_Test(&request,&done,&status);
      ++i;
      inrange=(i<=((rank+1)*nvalues-1) && i>=rank*nvalues);
  }
  if (!myfound) {
    printf("P:%d stopped at Array index %d\n",rank,i-1);
  }
  MPI_Finalize();
}
