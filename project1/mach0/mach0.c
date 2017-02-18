#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double arctan(double x, double number_of_iterations);
double estimate_pi(double number_of_iterations);

double estimate_pi(double number_of_iterations){

    double arctan1_5, arctan1_239, pi;

    arctan1_5 = arctan( 0.2, number_of_iterations);
    arctan1_239 = arctan( (double) 1/239, number_of_iterations);
    pi = 16.0*arctan1_5 - (4.0*arctan1_239);

    return pi;
}


double arctan(double x, double number_of_iterations){
    double mach_sum = 0;

    for (double iteration = 0; iteration < number_of_iterations; iteration++) {
        if ((int)iteration%2) {
            mach_sum -= pow(x,((2.0*iteration) + 1)) / ((2.0*iteration) + 1);
        }
        else{
            mach_sum += pow(x,(2.0*iteration + 1)) / (2.0*iteration + 1);
        }
    }

    return mach_sum;

}
