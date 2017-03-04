#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

    int num_iter = (int) number_of_iterations;

    #pragma omp parallel for reduction(+:zeta_sum)
    for (int i = 1; i <= num_iter; i++) {
        double iteration = (double) i;
        zeta_sum += 1.0/(iteration*iteration);
    }

    return zeta_sum;
}
