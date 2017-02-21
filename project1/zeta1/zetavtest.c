#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "zeta1.c"

double* sum_elements1_239;
double* sum_elements1_5;

const double PI = 3.141592653589793238463;


int main ( int argc, char **argv ){

    int size, rank;
    double pi_estimate, error;
    int number_of_iterations;
    int number_of_tests = 29;

    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    if (rank == 0) {
        printf("Running verification tests for pi estimation with riemann zeta function using MPI\n");
    }

    for (int k = 1; k <= number_of_tests; k++) {
        number_of_iterations = pow(2,k);
        pi_estimate = estimate_pi(size, rank, number_of_iterations);
        error = PI - pi_estimate;
        if (rank == 0) {
            printf("Test : %.d\tNumber of iteration : %10d\tEstimate : %.16f\t\tError : %.16f\n", k, number_of_iterations, pi_estimate, error);
        }
    }


    MPI_Finalize();

    return 0;
}
