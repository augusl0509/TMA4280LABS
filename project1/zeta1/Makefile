CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm
TARGETS=estimatepi zetavtest
NP=4
NITER = 1000000

all: ${TARGETS}

run: ${TARGETS}
	mpirun -np ${NP} estimatepi ${NITER}

vtest: ${TARGETS}
	mpirun -np ${NP} zetavtest

clean:
	-rm -f ${TARGETS}
