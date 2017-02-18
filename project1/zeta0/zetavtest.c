#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zeta0.c"

const double PI = 3.141592653589793238463;

int main(int argc, char **argv) {

    double number_of_iterations, pi_estimate, error;
    int number_of_tests = 24;

    printf("Running verification tests for pi estimation with riemann zeta function\n");

    for (double k = 1; k <= number_of_tests; k++) {
        number_of_iterations = pow(2,k);
        pi_estimate = estimate_pi(number_of_iterations);
        error = PI - pi_estimate;

        printf("Test : %.d\tNumber of iteration : %10d\tEstimate : %.10f\t\tError : %.10f\n", (int) k, (int) number_of_iterations, pi_estimate, error);
    }

    return 0;
}
