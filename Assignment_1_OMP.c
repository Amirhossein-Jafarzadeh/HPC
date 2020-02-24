#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
int main(){
	srand(time(NULL));
	clock_t start_1, stop_1;
	start_1 = clock();
	double start_2, stop_2, wtime_2;
	start_2 = omp_get_wtime();
	int i,j;
	int M = pow(2,15);
	int N = pow(2,15);
	int **Matrix = (int **)malloc(M * sizeof(int *));
    	for (i=0; i<M; i++)
        	Matrix[i] = (int *)malloc(N * sizeof(int));
	int *Vector = malloc(N * sizeof(int *));
	int *Result = malloc(N * sizeof(int *));

	//srand(time(NULL));
	//clock_t start_1, stop_1;	
	int sum;

	for(i=0; i<M; i++){
		for(j=0; j<N; j++)
			Matrix[i][j]=rand()%100;
	}

	for(j=0; j<N; j++)
		Vector[j]=rand()%100;
	//omp_set_num_threads(4);
	#pragma omp parallel for\
	private(i,j,sum) shared(M, N, Matrix, Vector, Result)
	for(i=0; i<M; i++){
		sum = 0;
		for(j=0; j<N; j++)
			{
			sum+= Matrix[i][j] * Vector[j];
			}
		Result[i]=sum;
		}
	stop_1 = clock();
	double wtime_1 = (double)(stop_1 - start_1)/CLOCKS_PER_SEC;
	printf("took %f sec\n", wtime_1);
	/*stop_2 = omp_get_wtime();
	wtime_2 = stop_2 - start_2;
	printf("took %f sec\n", wtime_2);*/
return 0;
}
