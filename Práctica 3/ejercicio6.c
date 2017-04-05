#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"

#define MUTEX 0
#define VACIO 1
#define LLENO 2
#define SIZE 5
#define KEY 1300
#define FILEKEY "/bin/cat"


int main(){
    int pid_productor, pid_consumidor;
    int semid;
    int key;
    int id_zone;
    char* buffer;
    char c;
    char* s;
    unsigned short array[] = {1,SIZE,0};

    key = ftok(FILEKEY, KEY);
    if(key == -1){
    printf("Error al obtener key\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }
                
    id_zone = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if(id_zone == -1){
        id_zone = shmget(key, SIZE, IPC_CREAT | SHM_R | SHM_W);
        if(id_zone == -1){
            printf("Error al crear la zona de memoria compartida\n");
            exit(EXIT_FAILURE);
        }
    }

    if(Crear_Semaforo(KEY, 3, &semid) == -1){
        perror("Error al crear el semaforo\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }

    if(Inicializar_Semaforo(semid, array) == ERROR){
        perror("Error al inicializar semáforos");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    if((pid_productor = fork()) == -1){
        perror("Error al realizar el primer fork\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    /* productor */
    if(pid_productor == 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }

        s = buffer;        
        for(c = 'a'; c <= 'z'; c++){
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
            *s++ = c;
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
    
    if((pid_consumidor = fork()) == -1){
        perror("Error al realizar el segundo fork\n");
        kill(pid_productor, SIG_KILL);
        wait(NULL);
        Borrar_Semaforo(semid);        
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }
    
    /* consumidor */
    if(pid == 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }

        for(s = buffer; *s <= 'z'; s++){
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
        }
        exit(EXIT_SUCCESS);
    }
    
    wait(NULL);
    wait(NULL);
    shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
    Borrar_Semaforo(semid);
    exit(EXIT_SUCCESS);
}
