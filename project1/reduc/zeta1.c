#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

int generate_zeta_elements(double start, double end, double* sum_elements);
double calc_pi_from_zetasum(double zetasum);
double sum_elements(double* elements, int number_of_elements);
int distribute_vector_elements(int size, int rank, double* global_array, double*local_array, int iterations);
int determine_local_iterations(int iterations, int size, int rank);
int determine_global_index(int number_of_iterations, int size, int rank);
int is_power_of_two(int number);
void recursive_doubling_sum(int rank, int size, double* local_riemann_zeta_sum);


int is_power_of_two(int number){
    return number && !(number & (number - 1));
}


int determine_local_iterations(int iterations, int size, int rank){
    int local_iter;
    if (rank <= (iterations % (size-1))) {
        local_iter = (iterations / (size-1)) + 1;
    }
    else{
        local_iter = iterations / (size-1);
    }

    return local_iter;
}

int determine_global_index(int number_of_iterations, int size, int rank){
    int global_index;
    if (rank <= number_of_iterations % (size - 1)) {
        global_index = (rank-1) * (number_of_iterations / (size - 1)) + (rank-1);
    }
    else{
        global_index = (rank-1) * (number_of_iterations / (size - 1)) + (number_of_iterations % (size - 1));
    }
    return global_index;
}

int distribute_vector_elements(int size, int rank, double* global_array, double* local_array, int iterations){
    int local_length = determine_local_iterations(iterations, size, rank);
    if (rank ==0) {

        int global_index = determine_global_index(iterations, size, rank);
        for (int i = 1; i < size; i++) {
            local_length = determine_local_iterations(iterations, size, i);
            global_index = determine_global_index(iterations, size, i);
            MPI_Ssend(&global_array[global_index], local_length, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(local_array, local_length, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, NULL);
    }
}

double sum_elements(double* elements, int number_of_elements){
    double sum = 0;

    for (int i = 0; i < number_of_elements; i++) {
        sum += elements[i];
    }
    return sum;
}

double calc_pi_from_zetasum(double zetasum){

    double pi;
    pi = sqrt(6.0 * zetasum) ;

    return pi;
}

int generate_zeta_elements(double start, double end, double* sum_elements){
    double iteration = start;

    for (iteration = 1; iteration <= end; iteration++) {
        sum_elements[(int)iteration-1] = 1 / (iteration * iteration);
    }
    return 0;
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
double* riemann_zeta_elements;

double estimate_pi( int size, int rank, int number_of_iterations ){

    int local_number_of_iterations;

    double pi, riemann_zeta_sum;
    double local_riemann_zeta_sum;

    double* local_riemann_zeta_elements;

    local_number_of_iterations = determine_local_iterations(number_of_iterations, size, rank);
    local_riemann_zeta_elements = (double*) malloc(local_number_of_iterations*sizeof(double));

    if (rank == 0) {
        riemann_zeta_elements = (double*) malloc( number_of_iterations*sizeof(double));
        generate_zeta_elements(0.0, (double) number_of_iterations, riemann_zeta_elements);
    }

    distribute_vector_elements(size, rank, riemann_zeta_elements, local_riemann_zeta_elements, number_of_iterations);

    local_riemann_zeta_sum = 0;
    if (rank != 0) {
        local_riemann_zeta_sum = sum_elements(local_riemann_zeta_elements, local_number_of_iterations);
    }

    //MPI_Allreduce(&local_riemann_zeta_sum, &riemann_zeta_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    recursive_doubling_sum(rank, size, &local_riemann_zeta_sum);

    pi = calc_pi_from_zetasum(local_riemann_zeta_sum);

    free(riemann_zeta_elements);
    free(local_riemann_zeta_elements);

    return pi;

}
