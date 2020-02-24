echo "------------------------------"
echo "HPC: Assignment 1, 2020"
echo "Amirhossein Jafarzadeh Ghazi"
echo "Student Number: 100745718"
echo "------------------------------"

for compiler_1 in gcc icc
  do
	   echo "The compiler used is $compiler_1:"
	     $compiler_1 Assignment_1.c -o Assignment_1
	      ./Assignment_1
  done

for compiler_2 in gcc icc
do
    for opt_1 in O0 O3
    do

	     echo "The compiler used is $compiler_2 and $opt_1 is optimization mode:"
	       $compiler_2 -$opt_1 Assignment_1.c -o Assignment_1
	        ./Assignment_1
    done
done

echo "OMP with the gcc compiler:"
		    gcc -fopenmp Assignment_1_OMP.c -o Assignment_1_OMP

	    for OMP_NUM_THREADS in 1 2 4
	    do
		      echo "Number of Threads is $OMP_NUM_THREADS"
		        export OMP_NUM_THREADS=$OMP_NUM_THREADS
		          ./Assignment_1_OMP
	    done

      echo "OMP with the icc compiler"
      		  icc -qopenmp Assignment_1_OMP.c
      	    for OMP_NUM_THREADS in 1 2 4
      	    do
      		      echo "Number of Threads is $OMP_NUM_THREADS"
      		        export OMP_NUM_THREADS=$OMP_NUM_THREADS
      		          ./Assignment_1_OMP
      	    done
