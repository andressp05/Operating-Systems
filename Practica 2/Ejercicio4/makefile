all: clean ejercicio4a ejercicio4b

CCFlags = -Wall -pedantic -g

ejercicio4a: ejercicio4a.o
	gcc -o ejercicio4a ejercicio4a.o -lpthread

ejercicio4b: ejercicio4b.o
	gcc -o ejercicio4b ejercicio4b.o -lpthread

ejercicio4a.o: ejercicio4a.c
	gcc $(CCFlags) -c  ejercicio4a.c

ejercicio4b.o: ejercicio4b.c
	gcc $(CCFlags) -c  ejercicio4b.c

clean:
	rm -rf *.o ejercicio4a ejercicio4b #*.tgz

#dist:
#	tar -cvzf G2202_P04_1.tgz *c makefile
