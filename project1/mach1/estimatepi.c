#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mach1.c"

int main(int argc, char **argv) {

    int size, rank;
    double pi, error;
    int number_of_iterations;

    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atoi(argv[1]);
    if (rank == 0) {
        printf("Number of iterations = %d\n", (int) number_of_iterations);
    }

    double start, end;

    start = MPI_Wtime();

    pi = estimate_pi(size, rank, number_of_iterations );

    end = MPI_Wtime();

    if (rank == 0) {
        printf("Pi : %.16f\tTime : %f\n\n", pi, end-start);
    }

    MPI_Finalize();

    return 0;
}
