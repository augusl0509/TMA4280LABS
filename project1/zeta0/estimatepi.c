#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "zeta0.c"

double walltime() {
    static struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + 1e-6 * t.tv_usec);
}

int main(int argc, char **argv) {

    double number_of_iterations;
    double zeta_sum, pi;

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atoi(argv[1]);

    printf("Number of iterations = %d\n", (int) number_of_iterations);

    double start, end;

    start = walltime();

    pi = estimate_pi(number_of_iterations);

    end = walltime();

    printf("Pi : %.16f\tTime : %f\n", pi, end-start);

    return 0;
}
