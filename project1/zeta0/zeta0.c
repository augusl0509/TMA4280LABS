#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double riemann_zeta_function(double number_of_iterations);
double estimate_pi(double number_of_iterations);


double estimate_pi(double number_of_iterations){
    double zeta_sum, pi;

    zeta_sum = riemann_zeta_function(number_of_iterations);
    pi = sqrt(6.0 * zeta_sum);

    return pi;
}

double riemann_zeta_function(double number_of_iterations){
    double zeta_sum = 0;

    for (double iteration = 1; iteration <= number_of_iterations; iteration++) {
        zeta_sum += 1.0/(iteration*iteration);
    }
    return zeta_sum;
}
