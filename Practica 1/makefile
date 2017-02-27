all: clean ejercicio4ai ejercicio4aii ejercicio4bi ejercicio4bii ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9 dist

CCFlags = -Wall -pedantic -ansi -g

ejercicio4ai: ejercicio4ai.o
	gcc -o ejercicio4ai ejercicio4ai.o
ejercicio4aii: ejercicio4aii.o
	gcc -o ejercicio4aii ejercicio4aii.o 
ejercicio4bi: ejercicio4bi.o
	gcc -o ejercicio4bi ejercicio4bi.o 
ejercicio4bii: ejercicio4bii.o
	gcc -o ejercicio4bii ejercicio4bii.o
ejercicio5a: ejercicio5a.o
	gcc -o ejercicio5a ejercicio5a.o
ejercicio5b: ejercicio5b.o
	gcc -o ejercicio5b ejercicio5b.o
ejercicio6: ejercicio6.o
	gcc -o ejercicio6 ejercicio6.o
ejercicio8: ejercicio8.o
	gcc -o ejercicio8 ejercicio8.o
ejercicio9: ejercicio9.o
	gcc -o ejercicio9 ejercicio9.o

ejercicio4ai.o: ejercicio4ai.c
	gcc $(CCFlags) -c  ejercicio4ai.c
ejercicio4aii.o: ejercicio4aii.c
	gcc $(CCFlags) -c ejercicio4aii.c 
ejercicio4bi.o: ejercicio4bi.c
	gcc $(CCFlags) -c ejercicio4bi.c 
ejercicio4bii.o: ejercicio4bii.c
	gcc $(CCFlags) -c ejercicio4bii.c
ejercicio5a.o: ejercicio5a.c
	gcc $(CCFlags) -c ejercicio5a.c
ejercicio5b.o: ejercicio5b.c
	gcc $(CCFlags) -c ejercicio5b.c
ejercicio6.o: ejercicio6.c
	gcc $(CCFlags) -c ejercicio6.c
ejercicio8.o: ejercicio8.c
	gcc $(CCFlags) -c ejercicio8.c
ejercicio9.o: ejercicio9.c
	gcc $(CCFlags) -c ejercicio9.c 

clean:
	rm -rf *.o ejercicio4ai ejercicio4aii ejercicio4bi ejercicio4bii ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9 *.tgz

dist:
	tar -cvzf G2202_P03_1.tgz *c makefile
