CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm
TARGETS=zetavtest machvtest
NP=4
all: ${TARGETS}

vtest: ${TARGETS}
	mpirun -np ${NP} zetavtest
	mpirun -np ${NP} machvtest

clean:
	-rm -f ${TARGETS}
