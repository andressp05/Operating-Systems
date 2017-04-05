all: clean ejercicio2 semaforos.o ejercicio5 #ejercicio6

CCFlags = -pedantic -g

ejercicio2: ejercicio2.o
	gcc -o ejercicio2 ejercicio2.o

ejercicio5: ejercicio5.o semaforos.o
	gcc -o ejercicio5 ejercicio5.o semaforos.o

ejercicio6: ejercicio6.o
	gcc -o ejercicio6 ejercicio6.o

ejercicio2.o: ejercicio2.c
	gcc $(CCFlags) -c  ejercicio2.c
 
semaforos.o: semaforos.c semaforos.h
	gcc $(CCFlags) -c semaforos.c

ejercicio6.o: ejercicio6.c semaforos.h
	gcc $(CCFlags) -c  ejercicio6.c

clean:
	rm -rf *.o ejercicio2 semaforos.o ejercicio5 ejercicio6 *.zip

dist:
	zip  G2202_P03_2.zip *c makefile Doxyfile *.txt *.pdf html/* latex/* html/search/*
