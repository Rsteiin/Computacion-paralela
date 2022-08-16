# Makefile para programas sum (cap 2)

SUMSSRC= sumSec.c
SUMSOBJ= sumSec.o
SUMPSRC= sumPar.c
SUMPOBJ= sumPar.o
FORKSRC= fork.c
FORKOBJ= fork.o
ALLOBJ= sumSec.o sumPar.o fork.o

all:    sumSec sumPar fork

sumSec:        $(SUMSOBJ)
	cc -o sumSec $(SUMSOBJ)

sumPar:        $(SUMPOBJ)
	cc -o sumPar $(SUMPOBJ)

fork:        $(FORKOBJ)
	cc -o fork $(FORKOBJ)

clean:	
	rm -f $(ALLOBJ)
