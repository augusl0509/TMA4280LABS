CFLAGS+=-std=c99 -O3
LDLIBS=-lm
TARGETS=estimatepi machutest machvtest
TEST_NITER = 3
NITER = 10000000

all: ${TARGETS}

utest: ${TARGETS}
	./machutest ${TEST_NITER}

vtest: ${TARGETS}
	./machvtest

run: ${TARGETS}
	./estimatepi ${NITER}

clean:
	-rm -f ${TARGETS}
