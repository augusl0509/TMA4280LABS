#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <mpi.h>
#include <omp.h>

#define PI 3.14159265358979323846
#define true 1
#define false 0

typedef double real;
typedef int bool;

// Function prototypes
real *mk_1D_array(size_t n, bool zero);
real **mk_2D_array(size_t n1, size_t n2, bool zero);
real rhs(real x, real y);

void fst_(real *v, int *n, real *w, int *nn);
void fstinv_(real *v, int *n, real *w, int *nn);

void find_local_start(int number_of_processes, int global_size, int** local_start, int** local_lenght);
void transpose(int rank, int size, int* local_start, int* local_lenght, real** b, int m);

real maximum_pointwise_error(int rank, int size, int m,int* local_start, int* local_lenght, real* grid, real** b);

void print_matrix(int rank, int size, int m, int* local_lenght, int* local_start, real** matrix);
