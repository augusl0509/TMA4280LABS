#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "mach2.c"

double* sum_elements1_239;
double* sum_elements1_5;

const double PI = 3.141592653589793238463;

int main ( int argc, char **argv ){

    int size, rank;
    double pi_estimate, error;
    int number_of_iterations;
    int number_of_tests = 24;

    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    if (rank == 0) {
        printf("Running verification tests for pi estimation with machin formula using MPI\n");
    }

    double start, end;

    for (int k = 1; k <= number_of_tests; k++) {
        number_of_iterations = pow(2,k);

        start = MPI_Wtime();

        pi_estimate = estimate_pi(size, rank, number_of_iterations);
        error = PI - pi_estimate;

        end = MPI_Wtime();

        if (rank == 0) {
            printf("Test : %.d\tNumber of iteration : %10d\tEstimate : %.16f\tError : %.16f\tTime : %f\n", k, number_of_iterations, pi_estimate, error, (end-start));
        }
    }

    MPI_Finalize();

    return 0;
}
