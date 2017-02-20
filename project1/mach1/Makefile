CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm
TARGETS=estimatepi machvtest
NP=4
all: ${TARGETS}

run: ${TARGETS}
	mpirun -np ${NP} estimatepi

vtest: ${TARGETS}
	mpirun -np ${NP} machvtest

clean:
	-rm -f ${TARGETS}
