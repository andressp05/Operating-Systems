all: clean ejercicio3a ejercicio3b

CCFlags = -Wall -pedantic -ansi -g

ejercicio3a: ejercicio3a.o
	gcc -o ejercicio3a ejercicio3a.o

ejercicio3b: ejercicio3b.o
	gcc -o ejercicio3b ejercicio3b.o -lpthread

ejercicio3a.o: ejercicio3a.c
	gcc $(CCFlags) -c  ejercicio3a.c

ejercicio3b.o: ejercicio3b.c
	gcc $(CCFlags) -c  ejercicio3b.c

clean:
	rm -rf *.o ejercicio3a ejercicio3b #*.tgz

#dist:
#	tar -cvzf G2202_P03_1.tgz *c makefile
