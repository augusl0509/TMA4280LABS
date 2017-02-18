CFLAGS+=-std=c99 -O3
LDLIBS=-lm
TARGETS=zeta0/zetautest mach0/machutest zeta0/estimatepi mach0/estimatepi
TEST_NITER = 3
NITER = 1000000

all: ${TARGETS}

utest:${TARGETS}
	./zeta0/zetautest ${TEST_NITER}
	./mach0/machutest ${TEST_NITER}

runzeta: ${TARGETS}
	./zeta0/estimatepi ${NITER}

runmach: ${TARGETS}
	./mach0/estimatepi ${NITER}

clean:
	-rm -f ${TARGETS}
