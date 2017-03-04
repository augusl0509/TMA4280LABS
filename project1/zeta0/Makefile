CFLAGS+=-std=c99 -O3
LDLIBS=-lm
TARGETS=estimatepi zetautest zetavtest
TEST_NITER = 3
NITER = 1000000000

all: ${TARGETS}

utest: ${TARGETS}
	./zetautest ${TEST_NITER}

vtest: ${TARGETS}
	./zetavtest

run: ${TARGETS}
	./estimatepi ${NITER}

clean:
	-rm -f ${TARGETS}
