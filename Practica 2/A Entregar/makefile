all: clean ejercicio3a ejercicio3b ejercicio4a ejercicio4b ejercicio6 ejercicio8 ejercicio10

CCFlags = -Wall -pedantic -ansi -g

ejercicio3a: ejercicio3a.o
	gcc -o ejercicio3a ejercicio3a.o

ejercicio3b: ejercicio3b.o
	gcc -o ejercicio3b ejercicio3b.o -lpthread

ejercicio4a: ejercicio4a.o
	gcc -o ejercicio4a ejercicio4a.o -lpthread

ejercicio4b: ejercicio4b.o
	gcc -o ejercicio4b ejercicio4b.o -lpthread

ejercicio6: ejercicio6.o
	gcc -o ejercicio6 ejercicio6.o

ejercicio3a.o: ejercicio3a.c
	gcc $(CCFlags) -c  ejercicio3a.c

ejercicio3b.o: ejercicio3b.c
	gcc $(CCFlags) -c  ejercicio3b.c

ejercicio4a.o: ejercicio4a.c
	gcc $(CCFlags) -c  ejercicio4a.c

ejercicio4b.o: ejercicio4b.c
	gcc $(CCFlags) -c  ejercicio4b.c

ejercicio6.o: ejercicio6.c
	gcc $(CCFlags) -c  ejercicio6.c

clean:
	rm -rf *.o ejercicio3a ejercicio3b ejercicio4a ejercicio4b ejercicio6 ejercicio8 ejercicio10 *.tgz

dist:
	zip  G2202_P03_2.zip *c makefile Doxyfile *.txt *.pdf html latex
