CC=mpicc
CFLAGS+=-std=c99 -O3
LDLIBS=-lm
TARGETS=estimatepi
NP=8
all: ${TARGETS}

run: ${TARGETS}
	mpirun -np ${NP} estimatepi

clean:
	-rm -f ${TARGETS}
