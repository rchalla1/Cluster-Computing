#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void Bucket_Sort(int array[], int ans[], int n)
{ 
    
    int i;  
    int count[n]; 
    for (i = 0; i < n; i++)
        count[i] = 0; 
    for (i = 0; i < n; i++)
        (count[array[i]])++;
    for (i = 1 ; i < n; i++)
        count[i]= count[i]+count[i-1]; 
    for (i = 0; i < n; i++)  
      {
       ans[count[array[i]]-1] = array[i];
       count[array[i]]= count[array[i]]- 1;
      }
    
} 	  
int main( int argc, char **argv[])
{
	int   i,k,n,tid,nthreads;  
	int array[50], ans[49];	
	printf("\n Enter the number of elements to be sorted :");
	scanf("%d",&n);
	printf("\n Enter the elements to be sorted :\n");
	for(i=0 ;  i < n ; i++){
	scanf("\n%d",&array[i]);
	}
	printf("Bucket Sorting Let's Begin..\n");
	
#pragma omp parallel shared(array,nthreads,ans,n,k) private(i,tid)
{
	tid = omp_get_thread_num();
	if (tid == 0)
    {
		nthreads = omp_get_num_threads();
		
		
    }
	#pragma omp 
	{
		Bucket_Sort(array,ans,n);		
			
	}
	
}
for(k=0;k<n;k++)
{
printf("%d\n",ans[k]);
}
printf("  Result of Bucket Sort with %d threads\n",nthreads);
}