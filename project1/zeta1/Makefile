CC=mpicc
CFLAGS+=-std=c99
LDLIBS=-lm
TARGETS=zetavtest
NP=4
all: ${TARGETS}

run: ${TARGETS}
	mpirun -np ${NP} zetavtest

vtest: ${TARGETS}
	mpirun -np ${NP} zetavtest

clean:
	-rm -f ${TARGETS}
