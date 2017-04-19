#include "poisson_mpilib.h"
#include <math.h>

real rhs(real x, real y) {
    return 5 * PI * PI * sin(PI * x) * sin(2 * PI * y);
}

real u(real x, real y){
    return sin(PI * x) * sin(2 * PI * y);
}

int main(int argc, char **argv) {

    int rank, size;
    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );

    if (argc < 2) {
        printf("Usage:\n");
        printf("  poisson n\n\n");
        printf("Arguments:\n");
        printf("  n: the problem size (must be a power of 2)\n");
    }

    //Read in problem size and number of processes.
    int n = atoi(argv[1]);
    int m = n-1;
    real h = 1.0/n;

    double start, end;
    start = MPI_Wtime();

    int* local_start;    //Which row/rows should this process have?
    int* local_lenght;
    //Each process should know what row/rows every process has so it knows where to send stuff
    //when we do the transpose of the matrix
    find_local_start(size, m, &local_start, &local_lenght);

    /*
     * Grid points are generated with constant mesh size on both x- and y-axis.
     */
    real *grid = mk_1D_array(n+1, false);
    for (size_t i = 0; i < n+1; i++) {
        grid[i] = i * h;
    }

    /*
     * The diagonal of the eigenvalue matrix of T is set with the eigenvalues
     * defined Chapter 9. page 93 of the Lecture Notes.
     * Note that the indexing starts from zero here, thus i+1.
     */

    //When the problem size is big
    //We could get a little speedup by distributing this calculation.
    //This should be easy because we have already divided m between the processes
    //The same goes for the grid points!

    real *diag = mk_1D_array(m, false);
    for (size_t i = 0; i < m; i++) {
        diag[i] = 2.0 * (1.0 - cos((i+1) * PI / n));
    }

    /*
     * Allocate the matrices b and bt which will be used for storing value of
     * G, \tilde G^T, \tilde U^T, U as described in Chapter 9. page 101.
     */
    real **b = mk_2D_array(local_lenght[rank], m, false);
    real **bt = mk_2D_array(local_lenght[rank], m, false);

    //This says the same as for the serial because we are working on rows,
    //which are the same lenght in both cases
    int nn = 4 * n;
    real *z = mk_1D_array(nn, false);

    /*
     * Initialize the right hand side data for a given rhs function.
     * Note that the right hand-side is set at nodes corresponding to degrees
     * of freedom, so it excludes the boundary (bug fixed by petterjf 2017).
     *
     */
    for (size_t i = 0; i < local_lenght[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            b[i][j] = h * h * rhs(grid[i + local_start[rank] + 1], grid[j+1]);
        }
    }

    //Each process calculates the DST from its part of the matrix
    for (size_t i = 0; i < local_lenght[rank]; i++) {
        fst_(b[i], &n, z, &nn);
    }

    transpose(rank, size, local_start, local_lenght, b, m);

    for (size_t i = 0; i < local_lenght[rank]; i++) {
        fstinv_(b[i], &n, z, &nn);
    }


    for (size_t i = 0; i < local_lenght[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            b[i][j] = b[i][j] / (diag[i + local_start[rank]] + diag[j]);
        }
    }

    for (size_t i = 0; i < local_lenght[rank]; i++) {
        fst_(b[i], &n, z, &nn);
    }
    transpose(rank, size, local_start, local_lenght, b, m);
    for (size_t i = 0; i < local_lenght[rank]; i++) {
        fstinv_(b[i], &n, z, &nn);
    }

    end = MPI_Wtime();

    real error = maximum_pointwise_error(rank, size, m,local_start, local_lenght, grid, b);


    if (rank == 0) {
        printf("Error : %.16f\tTime : %f\n\n", error, end-start);
    }

    MPI_Finalize();

    return 0;
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
    // print_matrix(rank, size, m, local_lenght, local_start, bt);

}

real maximum_pointwise_error(int rank, int size, int m, int* local_start, int* local_lenght, real* grid, real** b){
    real current_max_error = -1;
    real error = -2;
    for (size_t i = 0; i < local_lenght[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            error = fabs(b[i][j]-u(grid[i + local_start[rank]+1],grid[j+1]));
            if (current_max_error < error) {
                current_max_error = error;
            }
        }
    }

    MPI_Reduce(&current_max_error, &error, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    return error;
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
