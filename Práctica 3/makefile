all: clean ejercicio2 semaforos.o ejercicio5 ejercicio5b ejercicio6 dist

CCFlags = -pedantic -g

ejercicio2: ejercicio2.o
	gcc -o ejercicio2 ejercicio2.o

ejercicio5: ejercicio5.o semaforos.o
	gcc -o ejercicio5 ejercicio5.o semaforos.o

ejercicio5b: ejercicio5b.o semaforos.o
	gcc -o ejercicio5b ejercicio5b.o semaforos.o

ejercicio6: ejercicio6.o semaforos.o
	gcc -o ejercicio6 ejercicio6.o semaforos.o

ejercicio2.o: ejercicio2.c
	gcc $(CCFlags) -c  ejercicio2.c
 
semaforos.o: semaforos.c semaforos.h
	gcc $(CCFlags) -c semaforos.c

clean:
	rm -rf *.o ejercicio2 ejercicio5 ejercicio5b ejercicio6 *.zip

dist:
	zip  G2202_P03_2.zip *c makefile Doxyfile *.txt *.pdf html/* latex/* html/search/*
