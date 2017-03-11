all: clean ejercicio6

CCFlags = -Wall -pedantic -g

ejercicio6: ejercicio6.o
	gcc -o ejercicio6 ejercicio6.o

ejercicio6.o: ejercicio6.c
	gcc $(CCFlags) -c  ejercicio6.c

clean:
	rm -rf *.o ejercicio6 #*.tgz

#dist:
#	tar -cvzf G2202_P04_1.tgz *c makefile
