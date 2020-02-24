#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/time.h>
#include <math.h>
int main(){
	srand(time(NULL));
	clock_t start, stop;
	start = clock();
	int i,j;
	int M = pow(2,15);
	int N = pow(2,15);
	int **Matrix = (int **)malloc(M * sizeof(int *)); 
    	for (i=0; i<M; i++) 
        	Matrix[i] = (int *)malloc(N * sizeof(int));  
	int *Vector = malloc(N * sizeof(int *));
	int *Result = malloc(N * sizeof(int *));

	for(i=0; i<M; i++){	
		for(j=0; j<N; j++)
			Matrix[i][j]=rand()%100;	
	}
	for(j=0; j<N; j++)
		Vector[j]=rand()%100;
	for(i=0; i<M; i++){
		int sum = 0;
		for(j=0; j<N; j++)
			{
			sum+= Matrix[i][j] * Vector[j];
			}
		Result[i]=sum;
		}
	stop = clock();
	double wtime = (double)(stop - start)/CLOCKS_PER_SEC;
	printf("took %f sec\n", wtime);
return 0;
}