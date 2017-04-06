/**
 * @brief Implementa el ejercicio 6 de semaforos (productor-consumidor)
 * @file ejercicio6.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 01/03/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"

/**
* @brief Definicion del numero de semaforo Mutual Exclusion (MUTEX)
*/
#define MUTEX 0

/**
* @brief Definicion del numero de semaforo Vacio
*/
#define VACIO 1

/**
* @brief Definicion del numero de semaforo Lleno
*/
#define LLENO 2

/**
* @brief definicion del tamaño del buffer
*/
#define SIZE 5

/**
* @brief Definicion de la clave
*/
#define KEY 1300

/**
* @brief Definicion de la clave de fichero
*/
#define FILEKEY "/bin/cat"

/**
* @brief funcion principal que implementa el problema
*   del productor-consumidor visto en teoria con un abecedario
* @return int: valor de exito o fracaso
*/
int main(){
    /*
    * Declaración de variables
    */
    int i; /* variable para convertir el buffer en circular*/
    int pid_productor, pid_consumidor; /* pid para manejar los hijos tras el fork*/
    int semid; /*Identificador de semaforos*/
    int key; /* Clave para crear zona compartida*/
    int id_zone; /* Identificador de la zona compartida */
    char* buffer; /* Array que ira almacenando las letras producidas*/
    char c; /* Caracter leido*/
    char* s; /* Variable para manejar caracteres del buffer*/
    unsigned short array[] = {1,SIZE-1,0}; /* Array de semaforos*/

    /*
    * Obtenemos la clave para poder crear la zona compartida
    */
    key = ftok(FILEKEY, KEY);
    if(key == -1){
    printf("Error al obtener key\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }
                
    /*
    * Creacion de la zona compartida
    */            
    id_zone = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if(id_zone == -1){
        id_zone = shmget(key, SIZE, IPC_CREAT | SHM_R | SHM_W);
        if(id_zone == -1){
            printf("Error al crear la zona de memoria compartida\n");
            exit(EXIT_FAILURE);
        }
    }

    /*
    * Creacion de semaforos
    */
    if(Crear_Semaforo(KEY, 3, &semid) == -1){
        perror("Error al crear el semaforo\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }

    /*
    * Inicializacion de los semaforos
    */
    if(Inicializar_Semaforo(semid, array) == ERROR){
        perror("Error al inicializar semáforos");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    /*
    * Creamos el primer proceso hijo que sera el productor
    */
    if((pid_productor = fork()) == -1){
        perror("Error al realizar el primer fork\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    /*
    * Trabajo que realiza el productor con semaforos 
    * No comentamos internamente porque el pseudocodigo se
    * estudio en la teoria
    */
    if(pid_productor == 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }

        s = buffer;        
        for(i = 1, c = 'a'; c <= 'z'; i++, c++){
            if(Down_Semaforo(semid, VACIO, 0) == ERROR){
                perror("Error al decrementar el semáforo vacio");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            if(Down_Semaforo(semid, MUTEX, 0) == ERROR){
                perror("Error al decrementar el semáforo MUTEX");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            *s = c;
            s = buffer+i%SIZE;
            printf("Produce %c\n", c);
            if(Up_Semaforo(semid, MUTEX, 0) == ERROR){
                perror("Error al aumentar el semáforo MUTEX");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            if(Up_Semaforo(semid, LLENO, 0) == ERROR){
                perror("Error al aumentar el semáforo lleno");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);
    }
    
    /*
    * Creamos el segundo proceso hijo que sera el consumidor
    */
    if((pid_consumidor = fork()) == -1){
        perror("Error al realizar el segundo fork\n");
        kill(pid_productor, SIGKILL);
        wait(NULL);
        Borrar_Semaforo(semid);        
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }
    
    /* 
    * Trabajo que realiza el consumidor con semaforos 
    * No comentamos internamente porque el pseudocodigo se
    * estudio en la teoria
    */
    if(pid_consumidor == 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }

        for(s = buffer, i = 1; *s <= 'z'; i++){
            if(Down_Semaforo(semid, LLENO, 0) == ERROR){
                perror("Error al decrementar el semáforo lleno");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            if(Down_Semaforo(semid, MUTEX, 0) == ERROR){
                perror("Error al decrementar el semáforo MUTEX");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            
            printf("%c consumido\n", *s);
            fflush(stdout);

            if(Up_Semaforo(semid, MUTEX, 0) == ERROR){
                perror("Error al aumentar el semáforo MUTEX");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            if(Up_Semaforo(semid, VACIO, 0) == ERROR){
                perror("Error al aumentar el semáforo vacio");
                shmdt(buffer);
                exit(EXIT_FAILURE);
            }
            if(*s == 'z'){
                break;
            }
            s = buffer+i%SIZE;
        }
        exit(EXIT_SUCCESS);
    }
    
    /*
    * Finalizacion del trabajo (eliminacion de todo lo usado)
    */
    wait(NULL);
    wait(NULL);
    shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
    Borrar_Semaforo(semid);
    exit(EXIT_SUCCESS);
}
