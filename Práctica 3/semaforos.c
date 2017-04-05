/**
 * @brief Implementa el ejercicio 4 de la biblioteca de semaforos
 * @file semaforos.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 31/03/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "semaforos.h"


/**
* @brief Inicializa los semaforos indicados.
* @param semid: Identificador del semaforo.
* @param array: Valores iniciales.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int Inicializar_Semaforo(int semid,unsigned short *array){
    
    int i;

    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg;


    arg.array = (unsigned short *) malloc (sizeof(short)* sizeof(array) / sizeof(short));
    
    if(!arg.array)
        return ERROR;

    for (i = 0; i <= (sizeof(array) / sizeof(short)); i++){
        arg.array[i] = array[i];
    }

    if (semctl (semid, sizeof(array) / sizeof(short), SETALL, arg) == -1){
        free(arg.array);
        return ERROR;
    }

    return OK;
}

/**
* @brief Borra un semaforo.
* @param semid: Identificador del semaforo.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int Borrar_Semaforo(int semid){
    
    if( semctl(semid,0,IPC_RMID) == -1)
        return ERROR;
    
    return OK;
}

/**
* @brief Crea un semaforo con la clave y el tamaño 
*   especificado. Lo inicializa a 0.
* @param key: Clave precompartida del semaforo.
* @param size: Tamaño del semaforo.
* @param semid: Identificador del semaforo.
* @return int: ERROR en caso de error, 0 si ha creado el semaforo, 
*   1 si ya estaba creado.
*/
int Crear_Semaforo(key_t key, int size, int *semid){
    *semid = semget(key, size, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    
    if((*semid == -1) && (errno == EEXIST)){
        *semid = semget(key,size,SHM_R|SHM_W);
        
        if (*semid == -1)
            return ERROR;
        
        return 1;
    }
    
    if(*semid == -1){
        return ERROR;
    }

    return 0;
}

/**
* @brief Baja el semaforo indicado
* @param semid: Identificador del semaforo.
* @param num_sem: Semaforo dentro del array.
* @param undo: Flag de modo persistente pese a finalización
*   abrupta.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int Down_Semaforo(int semid, int num_sem, int undo){
    
    struct sembuf sem_oper;

    sem_oper.sem_num = num_sem;
    sem_oper.sem_op = -1;
    sem_oper.sem_flg = undo;

    if (semop(semid, &sem_oper, 1) == -1)
        return ERROR;

    return OK;
}

/**
* @brief Baja todos los semaforos del array indicado por active.
* @param semid: Identificador del semaforo.
* @param size: Numero de semaforos del array.
* @param undo: Flag de modo persistente pese a finalización
*   abrupta.
* @param active: Semaforos involucrados.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int DownMultiple_Semaforo(int semid,int size,int undo,int *active){
    
    int i,j;

    for(i=0; i< size; i++){
        if(Down_Semaforo(semid, active[i], undo) == ERROR){
            for(j = 0; j< i ; j++){
                if(Up_Semaforo(semid, active[j], undo) == ERROR)
                    return ERROR_SIN_REESTABLECER;  
            }
            return ERROR;
        }
    }

    return OK;
}

/**
* @brief Sube el semaforo indicado
* @param semid: Identificador del semaforo.
* @param num_sem: Semaforo dentro del array.
* @param undo: Flag de modo persistente pese a finalizacion
*   abupta.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int Up_Semaforo(int semid, int num_sem, int undo){

    struct sembuf sem_oper;

    sem_oper.sem_num = num_sem;
    sem_oper.sem_op = 1;
    sem_oper.sem_flg = undo;

    if (semop(semid, &sem_oper, 1) == -1)
        return ERROR;

    return OK;
}

/**
* @brief Sube todos los semaforos del array indicado por active.
* @param semid: Identificador del semaforo.
* @param size: Numero de semaforos del array.
* @param undo: Flag de modo persistente pese a finalización
*   abrupta.
* @param active: Semaforos involucrados.
* @return int: OK si todo fue correcto, ERROR en caso de error.
*/
int UpMultiple_Semaforo(int semid,int size, int undo, int *active){

    int i, j;

    for(i = 0; i< size; i++){
        if(Up_Semaforo(semid, active[i], undo) == ERROR){
            for(j = 0; j< i ; j++){
                if (Down_Semaforo(semid, active[j], undo) == ERROR)
                    return ERROR_SIN_REESTABLECER;  
            }
            return ERROR;
        }
    }

    return OK;  
}