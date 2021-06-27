CC=gcc -Wall -I.  -O2

ALL: clean svar2-test
svar2-test: mls.o svar2.o

clean:
	-${RM} *.o svar2-test
