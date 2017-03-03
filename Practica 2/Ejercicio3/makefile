all: clean ejercicio3a

CCFlags = -Wall -pedantic -ansi -g

ejercicio3a: ejercicio3a.o
	gcc -o ejercicio3a ejercicio3a.o

ejercicio3a.o: ejercicio3a.c
	gcc $(CCFlags) -c  ejercicio3a.c

clean:
	rm -rf *.o ejercicio3a #*.tgz

#dist:
#	tar -cvzf G2202_P03_1.tgz *c makefile
