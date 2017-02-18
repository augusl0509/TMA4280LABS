#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {

    double number_of_iterations;
    double zeta_sum, pi;

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atoi(argv[1]);

    printf("Number of iterations = %d\n", (int) number_of_iterations);

    zeta_sum = 0;

    for (double iteration = 1; iteration < number_of_iterations; iteration++) {
        zeta_sum += 1.0/(iteration*iteration);
    }

    pi = sqrt(zeta_sum*6.0);

    printf("Riemann zeta sum : %.10f\n", zeta_sum);
    printf("Pi : %.10f\n", pi);

    return 0;
}
