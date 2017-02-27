#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

double arctan(double x, double start, double end, double* sum_elements);
double calc_pi_from_arctan(double arctan1_5, double arctan1_239);
double sum_elements(double* elements, int number_of_elements);
int distribute_vector_elements(int size, int rank, double* global_array, double*local_array, int local_array_length);
int determine_local_iterations(int iterations, int size);
void recursive_doubling_sum(int rank, int size, double* local_riemann_zeta_sum);
int is_power_of_two(int number);


int is_power_of_two(int number){
    return number && !(number & (number - 1));
}


int determine_local_iterations(int iterations, int size){
    if (!is_power_of_two(size)) {
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

double calc_pi_from_arctan(double arctan1_5, double arctan1_239){

    double pi;
    pi = 16.0*arctan1_5 - (4.0*arctan1_239);

    return pi;
}

double arctan(double x, double start, double end, double* sum_elements){
    double mach_sum = 0;
    double iteration = start;

    for (iteration = 0; iteration < end; iteration++) {
        if ((int)iteration%2) {
            sum_elements[(int)iteration] = -pow(x,((2.0*iteration) + 1)) / ((2.0*iteration) + 1);
        }
        else{
            sum_elements[(int)iteration] = pow(x,(2.0*iteration + 1)) / (2.0*iteration + 1);
        }
    }

    return mach_sum;
}

void recursive_doubling_sum(int rank, int size, double* local_riemann_zeta_sum){

    double temporary_sum;
    int destination, source;

    for (int i = 0; i < (int) log2(size); i++) {
        if ((rank % (int) pow(2,i+1)) < pow(2,i)) {
            destination = (int)(rank + pow(2,i));
            source =  (int)(rank + pow(2,i));

            MPI_Sendrecv(
                local_riemann_zeta_sum, 1, MPI_DOUBLE, destination, 1,
                &temporary_sum, 1, MPI_DOUBLE, source, 1,
                MPI_COMM_WORLD, NULL);

                *local_riemann_zeta_sum += temporary_sum;
        }
        else{
            destination = (int)(rank - pow(2,i));
            source = (int)(rank - pow(2,i));

            MPI_Sendrecv(
                local_riemann_zeta_sum, 1, MPI_DOUBLE, destination, 1,
                &temporary_sum, 1, MPI_DOUBLE, source, 1,
                MPI_COMM_WORLD, NULL);

                *local_riemann_zeta_sum += temporary_sum;
        }
    }
}


double* sum_elements1_239;
double* sum_elements1_5;
double estimate_pi( int size, int rank, int number_of_iterations ){

    int local_number_of_iterations;

    double pi, arctan1_5, arctan1_239;
    double local_sum1_5, local_sum1_239;

    double* local_sum_elements1_5;
    double* local_sum_elements1_239;

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

    //MPI_Reduce(&local_sum1_5, &arctan1_5, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    //MPI_Reduce(&local_sum1_239, &arctan1_239, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //MPI_Allreduce(&local_sum1_5, &arctan1_5, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    //MPI_Allreduce(&local_sum1_239, &arctan1_239, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    recursive_doubling_sum(rank, size, &local_sum1_5);
    recursive_doubling_sum(rank, size, &local_sum1_239);

    pi = calc_pi_from_arctan(local_sum1_5, local_sum1_239);

    free(sum_elements1_5);
    free(sum_elements1_239);
    free(local_sum_elements1_5);
    free(local_sum_elements1_239);

    return pi;

}
