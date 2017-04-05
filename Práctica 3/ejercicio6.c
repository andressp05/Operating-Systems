#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MUTEX 0
#define VACIO 1
#define LLENO 2
#define SIZE 26
#define KEY 1300
#define FILEKEY "/bin/cat"


int main(){
    int pid;
    int semid;
    int key;
    int id_zone;
    char* buffer;
    
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

    semid = crear_semaforo(3, 1-MUTEX, 2-26-ario, inicializo 1-26-0);
    if(semid == -1){
        perror("Error al crear el semaforo\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) == -1){
        perror("Error al realizar el primer fork\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }
    
    if(pid > 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }
        
        while(1){
            Down_Semaforo(VACIO);
            Down_Semaforo(MUTEX);
            escribe;
            Up_Semaforo(MUTEX);
            Up_Semaforo(LLENO);
        }
        exit(EXIT_SUCCESS);
    }
    
    if((pid = fork()) == -1){
        perror("Error al realizar el primer fork\n");
        shmdt(buffer);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
        exit(EXIT_FAILURE);
    }
    
    if(pid > 0){
        buffer = shmat(id_zone, (char*)0, SHM_R | SHM_W);
        if(buffer == NULL){
            printf("Error al unir la memoria\n");
            exit(EXIT_FAILURE);
        }
        
        while(1){
            Down_Semaforo(LLENO);
            Down_Semaforo(MUTEX);
            lee;
            Up_Semaforo(MUTEX);
            Up_Semaforo(VACIO);
        }
        exit(EXIT_SUCCESS);
    }
    
    wait();
    wait();
    exit(EXIT_SUCCESS);
}