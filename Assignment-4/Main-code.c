#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <time.h>
#include <mpi.h>
#include "mkl.h"
#define MPI double
int main(int argc, char **argv){

// MPI INITIALIZATION

	int my_id, root_process, ierr, num_procs;
	MPI_Status status;

	ierr = MPI_Init(&argc, &argv);

// ALL PROCESSES RUN THEIR INTIALIZATION

	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id); //Find out MY process ID, and how many processes were started
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	int size, samples, bins;
	int my_samples;

	size=1000;
	samples=100;
	bins=25;

// MASTER RUNNING A ROUND ROBIN OF HOW MANY SAMPLES EACH SLAVE HAS TO CALCULATE

	if(my_id==0){

		int min_samp, extra_samp,other_samp;

		min_samp = samples/num_procs; // integer division
		extra_samp = samples%num_procs; // modulo division
		other_samp = min_samp+1;

		for(int i=0;i<extra_samp;i++){ // The first extra_samp processes get 1 extra sample than rest
			if(i==0){
				my_samples=other_samp;
			}
			else{
				// Send number of samples for each specific slave
				ierr = MPI_Send(&other_samp,1,MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}

		for(int j=extra_samp;j<num_procs;j++){ // The remaining processes get 1 less sample
			if(j==0){
				my_samples=min_samp;
			}
			else{
				// Send number of samples for each specific slave
				ierr = MPI_Send(&min_samp,1,MPI_INT, j, 0, MPI_COMM_WORLD);
			}
		}
	}

// SLAVES RECEIVING THE RESULTS OF THE ROUND ROBIN

	else{
		// Receive number of samples from master
		ierr = MPI_Recv(&my_samples, 1 , MPI_INT,0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}

// ALL PROCESSES RUN THIS CODE: CALCULATING MAX EIGENVALUE FOR EACH OF THEIR SAMPLES

	double *eigens=(double*)malloc(my_samples*sizeof(double)); // Used for storing max eigen values for each sample
	double *A=(double*)malloc(size*size*sizeof(double)); // Random matrix

	double *work=NULL;
	double *w=(double*)malloc(size*sizeof(double));

	int n,lwork,info;
	work = (double *)malloc(1*sizeof(double));

	#define METHOD VSL_RNG_METHOD_GAUSSIAN_ICDF // Initialized parameters for PRN generator
	VSLStreamStatePtr stream;
	double mean=0.0,sigma2=2.0, sigma=1.0;
	int diag_randos,other_randos, diag_index,other_index;
	double SEED, SEED2;

	diag_randos=size; // Num elements on diagonal
	other_randos=(int)(size*size-size)/2; // Num distinct non diagonal elements
	double *diag_num=(double*)malloc(diag_randos*sizeof(double));
	double *other_num=(double*)malloc(other_randos*sizeof(double));

	srand(my_id+1); // Seed the C random number generator to have different seeds across processes

	for(int s=0; s<my_samples; s++){

		SEED=rand(); // This will ensure different seeds across multiple calls to the MKL generator
		vslNewStream( &stream, VSL_BRNG_MCG31, SEED );
		vdRngGaussian( METHOD, stream, other_randos, other_num, mean, sigma); // Returns list to file upper triangle

		SEED2=rand();
		vslNewStream( &stream, VSL_BRNG_MCG31, SEED2);
		vdRngGaussian( METHOD, stream, diag_randos, diag_num, mean, sigma2); // Returns list to fill diagonal

		diag_index=0;
		other_index=0;

// FILLING MATRIX A WITH RANDOM NUMBERS GENERATED
	int o=1, d=1;
	for(int i=1; i<=size; i++){
		for(int j=0;j<=size-i+1;j++){
			A[i,j]=other_num[o];
			A[j,i]=A[i,j];
			if(i==j)
				A[i,j]=diag_num[d];
			o++;
			d++;
		}
	}

// COMPUTING ALL EIGENVALUES AND TAKING THE LARGEST

		lwork=-1;
		dsyev_( "N", "L", &size, A, &size, w, work, &lwork, &info);
		lwork= work[0];
		free(work);
		work = (double *)malloc(lwork*sizeof(double));

		dsyev_( "N", "L", &size, A, &size, w, work, &lwork, &info );

		eigens[s]=w[size-1];
	}

// SLAVES SENDING EIGENVALUE ARRAY BACK TO MASTER PROCESS

	if(my_id!=0){
		ierr = MPI_Send(&my_samples,1,MPI_INT, 0, 0, MPI_COMM_WORLD); // Send master how many eigenvalues slave has
		ierr = MPI_Send(eigens,my_samples,MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); // Send master eigenvalues
		printf("sent, %f\n",eigens[0]);
	}

// MASTER RECEIVES EIGENVALUES AND PUTS THEM INTO A DISTRIBUTION

	else{
		double *ALL_eigens=(double*)malloc(samples*sizeof(double)); // Used for storing all of the eigenvalues
		int *sort_flag=(int*)malloc(samples*sizeof(int)); // Used for storing all of the eigenvalues
		int *bin_count=(int*)malloc(bins*sizeof(int));
		double *histo_norm=(double*)malloc(bins*sizeof(double));

		int slave_samples;
		int total_index;
		double max, min, bin_width, left_edge,right_edge;
		max=0.0; // Assigning arbritrary max/min values to overwrite later
		min=10000;
		total_index=0;

		for(int k=1; k<num_procs; k++){
			ierr = MPI_Recv(&slave_samples, 1 , MPI_INT,k,0, MPI_COMM_WORLD, &status); // Receives number of eigenvalues slave has
			ierr = MPI_Recv(&ALL_eigens[total_index], slave_samples, MPI_DOUBLE,k,0, MPI_COMM_WORLD, &status); // Receives eigenvalue array
			total_index+=slave_samples; // Indexing the number of total samples received
		}

		for(int g=0;g<my_samples;g++){ // Puts masters eigenvalues in the total array
			ALL_eigens[total_index+g]=eigens[g];
		}

		for(int i=0;i<samples;i++)
			printf("%f \n", ALL_eigens[i]);

	free(A);
	free(eigens);
	ierr = MPI_Finalize();
}
}
