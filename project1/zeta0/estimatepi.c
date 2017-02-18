#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zeta0.c"


int main(int argc, char **argv) {

    double number_of_iterations;
    double zeta_sum, pi;

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atoi(argv[1]);

    printf("Number of iterations = %d\n", (int) number_of_iterations);

    pi = estimate_pi(number_of_iterations);

    printf("Pi : %.10f\n", pi);

    return 0;
}
