#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mach0.c"

int main(int argc, char **argv) {

    double number_of_iterations;
    double arctan1_5, arctan1_239, pi;

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atof(argv[1]);

    printf("Number of iterations = %d\n", (int) number_of_iterations);

    pi = estimate_pi(number_of_iterations);

    printf("Pi : %.10f\n", pi);

    return 0;
}
