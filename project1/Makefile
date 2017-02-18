CFLAGS+=-std=c99 -O3
LDLIBS=-lm
TARGETS=zeta0/zeta0 mach0/mach0
NITER = 3

all: ${TARGETS}

runzeta: ${TARGETS}
	./zeta0/zeta0 ${NITER}

runmach: ${TARGETS}
	./mach0/mach0 ${NITER}

clean:
	-rm -f ${TARGETS}
