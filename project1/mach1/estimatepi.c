#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

double arctan(double x, double start, double end, double* sum_elements);
double estimate_pi(double arctan1_5, double arctan1_239);
double sum_elements(double* elements, int number_of_elements);
int distribute_vector_elements(int size, int rank, double* global_array, double*local_array, int local_array_length);
int determine_local_iterations(int iterations, int size);


double pi, arctan1_5, arctan1_239;
double
    *sum_elements1_5,
    *sum_elements1_239,
    *local_sum_elements1_5,
    *local_sum_elements1_239;


int size, rank;

int main ( int argc, char **argv ){

    int number_of_iterations;
    int local_number_of_iterations;
    double local_sum1_5, local_sum1_239;

    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    number_of_iterations = 131072;

    local_number_of_iterations = determine_local_iterations(number_of_iterations, size);

    local_sum_elements1_5 = (double*) malloc(local_number_of_iterations*sizeof(double));
    local_sum_elements1_239 = (double*) malloc(local_number_of_iterations*sizeof(double));

    if (rank == 0) {
        sum_elements1_5 = (double*) malloc( number_of_iterations*sizeof(double));
        arctan( 0.2, 0.0, (double) number_of_iterations, sum_elements1_5);

        sum_elements1_239 = (double*) malloc( number_of_iterations*sizeof(double));
        arctan( (double) 1/239, 0.0, (double) number_of_iterations, sum_elements1_239);
    }

    distribute_vector_elements(size, rank, sum_elements1_5, local_sum_elements1_5, local_number_of_iterations);
    distribute_vector_elements(size, rank, sum_elements1_239, local_sum_elements1_239, local_number_of_iterations);

    local_sum1_5 = sum_elements(local_sum_elements1_5, local_number_of_iterations);
    local_sum1_239 = sum_elements(local_sum_elements1_239, local_number_of_iterations);

    MPI_Reduce(&local_sum1_5, &arctan1_5, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sum1_239, &arctan1_239, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank ==0) {
        printf("Arctan (1/5) : %.40f\tArctan (1/239) : %.40f\n", arctan1_5, arctan1_239);
        pi = estimate_pi(arctan1_5, arctan1_239);
        printf("Pi : %.40f\n", pi);
    }


    free(sum_elements1_5);
    free(sum_elements1_239);
    free(local_sum_elements1_5);
    free(local_sum_elements1_239);

    MPI_Finalize();

    return 0;

}

int determine_local_iterations(int iterations, int size){
    if ((int)iterations%size != 0 ) {
        fprintf(stderr, "Both the number of processes and the number of interations has to be a power of 2!\n");
        exit(1);
    }
    return iterations/size;
}

int distribute_vector_elements(int size, int rank, double* global_array, double*local_array, int local_array_length){
    if (rank ==0) {
        MPI_Sendrecv(
            global_array, local_array_length, MPI_DOUBLE, 0, 1,
            local_array, local_array_length, MPI_DOUBLE, 0, 1,
            MPI_COMM_WORLD, NULL);

        for (int i = 1; i < size; i++) {
            MPI_Send(&global_array[i*local_array_length], local_array_length, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(local_array, local_array_length, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, NULL);
    }
}

double sum_elements(double* elements, int number_of_elements){
    double sum = 0;

    for (int i = 0; i < number_of_elements; i++) {
        sum += elements[i];
    }
    return sum;
}

double estimate_pi(double arctan1_5, double arctan1_239){

    double pi;
    pi = 16.0*arctan1_5 - (4.0*arctan1_239);
    return pi;
}


double arctan(double x, double start, double end, double* sum_elements){
    double mach_sum = 0;
    double iteration = start;

    for (iteration = 0; iteration < end; iteration++) {
        if ((int)iteration%2) {
            sum_elements[(int)iteration] -= pow(x,((2.0*iteration) + 1)) / ((2.0*iteration) + 1);
        }
        else{
            sum_elements[(int)iteration] += pow(x,(2.0*iteration + 1)) / (2.0*iteration + 1);
        }
    }

    return mach_sum;

}
