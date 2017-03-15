all: clean ejercicio10

CCFlags = -pedantic -g

ejercicio10: ejercicio10.o
	gcc -o ejercicio10 ejercicio10.o

ejercicio10.o: ejercicio10.c
	gcc $(CCFlags) -c  ejercicio10.c

clean:
	rm -rf *.o ejercicio10  #*.tgz

#dist:
#	tar -cvzf G2202_P03_1.tgz *c makefile
