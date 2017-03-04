CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm -fopenmp
TARGETS=machvtest
NP=4
all: ${TARGETS}

vtest: ${TARGETS}
	mpirun -np ${NP} machvtest

clean:
	-rm -f ${TARGETS}
