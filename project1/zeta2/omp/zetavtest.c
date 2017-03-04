#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#include "zeta0.c"

const double PI = 3.141592653589793238463;

double walltime() {
    static struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + 1e-6 * t.tv_usec);
}

int main(int argc, char **argv) {

    double number_of_iterations, pi_estimate, error;
    int number_of_tests = 24;

    printf("Running verification tests for pi estimation with riemann zeta function\n");

    double start, end;

    for (double k = 1; k <= number_of_tests; k++) {
        number_of_iterations = pow(2,k);

        start = walltime();

        pi_estimate = estimate_pi(number_of_iterations);
        error = PI - pi_estimate;

        end = walltime();

        printf("Test : %2d\tNumber of iteration : %8d\tEstimate : %.16f\tError : %.16f\tTime : %f\n", (int) k, (int) number_of_iterations, pi_estimate, error, (end-start));
    }

    return 0;
}
