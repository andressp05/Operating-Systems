#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <msg.h>

#define ERROR -1

void ventanilla(void* arg); /* Hilo de las ventanillas */

void pantalla(void* args); /* Hilo del monitor */

void gestor(int numApostadores); /* Funcion que ejecuta el proceso hijo gestor de apuestas. Crea la cola de mensajes y un proceso hijo apostador ejecuta la funcion apostador */

void apostador(int msgid, int numApostadores, int numCaballos); /* Funcion que ejecuta el proceso apostador. Recibe el id de la cola de mensajes */

/* Funcion ejecutada por cada hijo caballo. Devuelve la tirada */
int caballo(int pos, int maxpos){
    if(pos > maxpos || pos <= 0){
        return ERROR;
    }

    if(pos == 1){
        return rand()%7+1;
    }

    if(pos == maxpos){
        return rand()%12+1;
    }

    return rand()%6+1;
}

void main(int argc, char* argv[]){
    int i;
    int numCaballos;
    int longitud;
    int apostadores;
    int ventanillas;
    int pid;
    int* caballos;
    int estado;

    if(argc != 4){
        printf("Se deben introducir cuatro parámetros enteros positivos:\nNúmero de caballos participantes (máximo 10)\n");
        printf("Longitud de la carrera\n, Número de apostadores (máximo 10)\n, Número de ventanillas\n");
        exit(EXIT_FAILURE);
    }

    if((numCaballos = atoi(argv[1]) <= 0 || numCaballos > 10){
        printf("El primer parámetro indica el número de caballos. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((longitud = atoi(argv[2]) <= 0){
        printf("El segundo parámetro indica la longitud de la carrera. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }

    if((apostadores = atoi(argv[3]) <= 0 || apostadores > 10){
        printf("El tercer parámetro indica el número de apostadores. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((ventanillas = atoi(argv[4]) <= 0){
        printf("El cuarto parámetro indica el número de ventanillas. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) == ERROR){
        printf("Error al crear el monitor\n");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0){
        /*MONITOR*/
    }

    if((pid = fork()) == ERROR){
        printf("Error al crear el gestor de apuestas\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        /*GESTOR*/
    }
    
    for(i = 0; i < numCaballos; i++){
        if((pid = fork()) == ERROR){
            printf("Error al crear el gestor de \n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            /*caballo*/
        }
    }
    sleep(15);
}

