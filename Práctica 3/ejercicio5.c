#include "semaforos.h"
#include <sys/sem.h>

#define SEMKEY 75798
#define N_SEMAFOROS 3

int main (){
	/*
	* Declaración de variables
	*/
	int sem_id_binario; /* IDs de semaforos binarios*/
	int ret;
	int *array;
	
	array = (int *)malloc(sizeof(int)*N_SEMAFOROS);
	if(!array){
		perror("Error al reservar memoria");
		return ERROR;
	}	
	/*
	* Creamos una lista o conjunto con tres semáforos
	*/
	if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id_binario) != 0){
		perror("Error al crear semáforos");
		free(array);
		return ERROR;
	}

	/*
	* Vemos que los semáforos ya se han creado
	*/
	if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id_binario) != 1){
		perror("Error al intentar crear semáforos ya existentes");
		free(array);
		return ERROR;
	}

	/*
	* Inicializamos los semáforos
	*/
	if(Inicializar_Semaforo(sem_id_binario, array) == ERROR){
		perror("Error al inicializar semáforos");
		free(array);
		return ERROR;
	}


	/*
	* Operamos sobre los semáforos
	*/
	
	if(Down_Semaforo(sem_id_binario, 0, SEM_UNDO) == ERROR){
		perror("Error al decrementar el semáforo 0");
		free(array);
		return ERROR;
	}


	if(Up_Semaforo(sem_id_binario, 0, SEM_UNDO) == ERROR){
		perror("Error al aumentar el semáforo 0");
		free(array);
		return ERROR;
	}	

	if(DownMultiple_Semaforo(sem_id_binario,N_SEMAFOROS,SEM_UNDO,array) == ERROR){
		perror("Error al decrementar el array de semáforos");
		free(array);
		return ERROR;
	}

	if(UpMultiple_Semaforo(sem_id_binario,N_SEMAFOROS,SEM_UNDO,array) == ERROR){
		perror("Error al aumentar el array de semáforos");
		free(array);
		return ERROR;
	}

	printf("Ejecución correcta");
	return OK;
	
}
/*	semctl (semid, N_SEMAFOROS, GETALL, arg);
	printf (“Los valores de los semáforos son %d y %d”,
	arg.array [0], arg.array [1]);
	/* Eliminar la lista de semáforos */
/*s	semctl (semid, N_SEMAFOROS, IPC_RMID, 0);
	free(arg.array);
	/* fin de la función main */