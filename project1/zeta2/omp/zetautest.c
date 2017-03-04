#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "zeta0.c"

int main(int argc, char **argv) {

    double number_of_iterations = 3;
    double pi_estimate, expected_result;

    expected_result = 2.85773803325;

    printf("Running unit test with %d iterations\n", (int) number_of_iterations);
    printf("Expected result is : %f\n", expected_result);

    pi_estimate = estimate_pi(number_of_iterations);

    printf("Result was : %.10f\n", pi_estimate);

    if (expected_result - pi_estimate <= 0.00000000001) {
        printf("Zeta unit test : Success\n");
    }
    else{
        printf("Zeta unit test : Fail\n");
    }

    return 0;
}
