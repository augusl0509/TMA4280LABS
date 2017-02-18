#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double arctan(double x, double number_of_iterations);

int main(int argc, char **argv) {

    double number_of_iterations;
    double arctan1_5, arctan1_239, pi;

    if (argc < 2) {
        fprintf(stderr, "You must pass in the number of iterations you want for the sum!\n");
        exit(1);
    }

    number_of_iterations = atof(argv[1]);

    printf("Number of iterations = %d\n", (int) number_of_iterations);

    arctan1_5 = arctan( 0.2, number_of_iterations);
    arctan1_239 = arctan( (double) 1/239, number_of_iterations);
    pi = 16.0*arctan1_5 - (4.0*arctan1_239);

    printf("Machin sum for x = 1/5 : %.10f\n", arctan1_5);
    printf("Machin sum for x = 1/239 : %.10f\n", arctan1_239);
    printf("Pi : %.10f\n", pi);

    return 0;
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
