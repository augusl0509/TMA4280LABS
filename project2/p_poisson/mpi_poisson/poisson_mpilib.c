#include "poisson_mpilib.h"

real rhs(real x, real y) {
    return 2 * (y - y*y + x - x*x);
}

//Calculates the starting row_index in the global grid
//and the number of rows each process is in charge of
void find_local_start(int number_of_processes, int global_size, int** local_start, int** local_lenght){

    int rows_per_process =  global_size / number_of_processes;
    int remaining_rows = global_size % number_of_processes;

    int* start = calloc(number_of_processes, sizeof(int));
    int* lenght = calloc(number_of_processes, sizeof(int));

    for (size_t i = 0; i < number_of_processes-1; i++) {
        int local_n_rows = rows_per_process;
        if (i<remaining_rows) {
            local_n_rows = rows_per_process + 1;
        }
        start[i+1] = start[i] + local_n_rows;
        lenght[i] = local_n_rows;
    }
    lenght[number_of_processes-1] = global_size - start[number_of_processes-1];

    *local_start = start;
    *local_lenght = lenght;
}

/*
 * The allocation of a vectore of size n is done with just allocating an array.
 * The only thing to notice here is the use of calloc to zero the array.
 */

real *mk_1D_array(size_t n, bool zero)
{
    if (zero) {
        return (real *)calloc(n, sizeof(real));
    }
    return (real *)malloc(n * sizeof(real));
}

/*
 * The allocation of the two-dimensional array used for storing matrices is done
 * in the following way for a matrix in R^(n1*n2):
 * 1. an array of pointers is allocated, one pointer for each row,
 * 2. a 'flat' array of size n1*n2 is allocated to ensure that the memory space
 *   is contigusous,
 * 3. pointers are set for each row to the address of first element.
 */

real **mk_2D_array(size_t n1, size_t n2, bool zero)
{
    // 1
    real **ret = (real **)malloc(n1 * sizeof(real *));

    // 2
    if (zero) {
        ret[0] = (real *)calloc(n1 * n2, sizeof(real));
    }
    else {
        ret[0] = (real *)malloc(n1 * n2 * sizeof(real));
    }

    // 3
    for (size_t i = 1; i < n1; i++) {
        ret[i] = ret[i-1] + n2;
    }
    return ret;
}


void transpose(int rank, int size, int* local_start, int* local_lenght, real** b, int m){

    real** sendbuffer = mk_2D_array(local_lenght[rank], m, false);
    real** recvbuffer = mk_2D_array(m, local_lenght[rank], false);

    int* sendcount = malloc(size * sizeof(int));
    int* sdispls = malloc(size * sizeof(int));

    for (size_t i = 0; i < size; i++) {
        sendcount[i] = local_lenght[rank] * local_lenght[i];
        sdispls[i] = local_lenght[rank] * local_start[i];
    }
    int* recvcount = malloc(size * sizeof(int));
    int* rdispls = malloc(size * sizeof(int));


    int x, y;
    x = 0;
    y = 0;
    for (size_t p = 0; p < size; p++) {
        for (size_t i = 0; i < local_lenght[rank]; i++) {
            for (size_t j = 0; j < local_lenght[p]; j++) {
                // printf("i = %lu, j = %lu\n", i+1, j+1+local_start[p]);
                sendbuffer[x][y] = b[i][j+local_start[p]];
                y++;
                if (y > m-1) {
                    y = 0;
                    x++;
                }
            }
        }
    }

    MPI_Alltoallv(  &sendbuffer[0][0], sendcount, sdispls, MPI_DOUBLE,
                    &recvbuffer[0][0], sendcount, sdispls, MPI_DOUBLE,
                    MPI_COMM_WORLD
    );

    for (size_t i = 0; i < local_lenght[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            b[i][j] = recvbuffer[j][i];
        }
    }
}

//Print for testing
void print_matrix(int rank, int size, int m, int* local_lenght, int* local_start, real** matrixpart){

    real** matrix;
    if (rank == 0) {
        matrix = mk_2D_array(m,m,false);
        for (size_t i = 0; i < local_lenght[rank]; i++) {
            for (size_t j = 0; j < m; j++) {
                matrix[i][j] = matrixpart[i][j];
            }
        }
    }

    if (rank !=0) {
        MPI_Ssend(&matrixpart[0][0], local_lenght[rank]*m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else{
        for (size_t i = 1; i < size; i++) {
            MPI_Recv(&matrix[local_start[i]][0], local_lenght[i] * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL);
        }
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < m; j++) {
                printf("%f\t", matrix[i][j]);
            }
            puts("");
        }
        puts("");
    }
}
