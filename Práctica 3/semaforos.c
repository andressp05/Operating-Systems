#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "semaforos.h"


/***************************************************************
Nombre:
Inicializar_Semaforo.
Descripcion:
Inicializa los semaforos indicados.
Entrada:
int semid: Identificador del semaforo.
unsigned short *array: Valores iniciales.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
************************************************************/
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

/***************************************************************
Nombre: Borrar_Semaforo.
Descripcion: Borra un semaforo.
Entrada:
int semid: Identificador del semaforo.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Borrar_Semaforo(int semid){
	
	if( semctl(semid,0,IPC_RMID) == -1)
		return ERROR;
	
	return OK;
}
/***************************************************************
Nombre: Crear_Semaforo.
Descripcion: Crea un semaforo con la clave y el tamaño
especificado. Lo inicializa a 0.
Entrada:
key_t key: Clave precompartida del semaforo.
int size: Tamaño del semaforo.
Salida:
int *semid: Identificador del semaforo.
int: ERROR en caso de error,
0 si ha creado el semaforo,
1 si ya estaba creado.
**************************************************************/
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
/**************************************************************
Nombre:Down_Semaforo
Descripcion: Baja el semaforo indicado
Entrada:
int semid: Identificador del semaforo.
int num_sem: Semaforo dentro del array.
int undo: Flag de modo persistente pese a finalización
abrupta.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Down_Semaforo(int semid, int num_sem, int undo){
	
	struct sembuf sem_oper;

	sem_oper.sem_num = num_sem;
	sem_oper.sem_op = -1;
	sem_oper.sem_flg = undo;

	if (semop(semid, &sem_oper, 1) == -1)
		return ERROR;

	return OK;
}

/***************************************************************
Nombre: DownMultiple_Semaforo
Descripcion: Baja todos los semaforos del array indicado
por active.
Entrada:
int semid: Identificador del semaforo.
int size: Numero de semaforos del array.
int undo: Flag de modo persistente pese a finalización
abrupta.
int *active: Semaforos involucrados.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
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

/**************************************************************
Nombre:Up_Semaforo
Descripcion: Sube el semaforo indicado
Entrada:
int semid: Identificador del semaforo.
int num_sem: Semaforo dentro del array.
int undo: Flag de modo persistente pese a finalizacion
abupta.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Up_Semaforo(int semid, int num_sem, int undo){

	struct sembuf sem_oper;

	sem_oper.sem_num = num_sem;
	sem_oper.sem_op = 1;
	sem_oper.sem_flg = undo;

	if (semop(semid, &sem_oper, 1) == -1)
		return ERROR;

	return OK;
}
/***************************************************************
Nombre: UpMultiple_Semaforo
Descripcion: Sube todos los semaforos del array indicado
por active.
Entrada:
int semid: Identificador del semaforo.
int size: Numero de semaforos del array.
int undo: Flag de modo persistente pese a finalización
abrupta.
int *active: Semaforos involucrados.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
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