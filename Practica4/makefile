all: clean cadena_montaje carrera dist

CCFlags = -g

cadena_montaje: cadena_montaje.o
	gcc -o cadena_montaje cadena_montaje.o -lm

cadena_montaje.o: cadena_montaje.c
	gcc $(CCFlags) -c  cadena_montaje.c
	
carrera: carrera.o
	gcc -o carrera carrera.o -lpthread

carrera.o: carrera.c
	gcc $(CCFlags) -c  carrera.c
 
clean:
	rm -rf *.o cadena_montaje carrera *.zip

dist:
	zip  G2202_P03_5.zip *.h *c makefile Doxyfile *.txt Informe.pdf html/* latex/* html/search/*
