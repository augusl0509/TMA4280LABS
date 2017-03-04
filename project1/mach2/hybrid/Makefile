CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm -fopenmp
TARGETS=estimatepi machvtest
NP=4
NITER = 1000000
all: ${TARGETS}

run: ${TARGETS}
	mpirun -np ${NP} estimatepi ${NITER}

vtest: ${TARGETS}
	mpirun -np ${NP} machvtest

clean:
	-rm -f ${TARGETS}
