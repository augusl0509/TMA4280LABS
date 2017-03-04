CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm -fopenmp
TARGETS=zetavtest
NP=4
all: ${TARGETS}

vtest: ${TARGETS}
	mpirun -np ${NP} zetavtest

clean:
	-rm -f ${TARGETS}
