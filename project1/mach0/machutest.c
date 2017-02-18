#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mach0.c"

int main(int argc, char **argv) {

    double number_of_iterations = 3;
    double pi_estimate, expected_result;

    expected_result = 3.14162102932;

    printf("Running unit test with %d iterations\n", (int) number_of_iterations);
    printf("Expected result is : %f\n", expected_result);

    pi_estimate = estimate_pi(number_of_iterations);

    printf("Result was : %.10f\n", pi_estimate);

    if (expected_result - pi_estimate <= 0.00000000001) {
        printf("Mach unit test : Success\n");
    }
    else{
        printf("Mach unit test : Fail\n");
    }

    return 0;
}
