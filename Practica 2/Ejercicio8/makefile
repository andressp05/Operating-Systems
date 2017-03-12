all: clean ejercicio8

CCFlags = -pedantic -ansi -g

ejercicio8: ejercicio8.o
	gcc -o ejercicio8 ejercicio8.o

ejercicio8.o: ejercicio8.c
	gcc $(CCFlags) -c  ejercicio8.c

clean:
	rm -rf *.o ejercicio8  #*.tgz

#dist:
#	tar -cvzf G2202_P03_1.tgz *c makefile
