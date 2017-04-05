#include "semaforos.h"
#include <sys/sem.h>

#define SEMKEY 75798
#define N_SEMAFOROS 3

int main (){
	/*
	* Declaración de variables
	*/
	int sem_id_binario; /* IDs de semaforos binarios*/
	int ret, i;
	unsigned short *array;
	int num_sem[] = {0, 1, 2};

	array = (unsigned short *)malloc(sizeof(short)*N_SEMAFOROS);
	if(!array){
		perror("Error al reservar memoria");
		return ERROR;
	}

	/*
	* Inicializamos el array de semaforos binarios
	*/
	for(i = 0; i < N_SEMAFOROS; i++){
		array[i] = 1;
	}	

	/*
	* Creamos una lista o conjunto con tres semáforos
	*/
	if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id_binario) != 0){
		perror("Error al crear semáforos");
		free(array);
		return ERROR;
	}

	printf("Crear\n");
	fflush(stdout);

	/*
	* Vemos que los semáforos ya se han creado
	*/
	if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id_binario) != 1){
		perror("Error al intentar crear semáforos ya existentes");
		free(array);
		return ERROR;
	}

	printf("Crear con semaforos ya existentes\n");
	fflush(stdout);

	/*
	* Inicializamos los semáforos
	*/
	if(Inicializar_Semaforo(sem_id_binario, array) == ERROR){
		perror("Error al inicializar semáforos");
		free(array);
		return ERROR;
	}

	printf("Inicializar\n");
	fflush(stdout);

	/*
	* Operamos sobre los semáforos
	*/
	if(Down_Semaforo(sem_id_binario, 0, SEM_UNDO) == ERROR){
		perror("Error al decrementar el semáforo 0");
		free(array);
		return ERROR;
	}

	printf("Down semaforo 0\n");
	fflush(stdout);

	if(Up_Semaforo(sem_id_binario, 0, SEM_UNDO) == ERROR){
		perror("Error al aumentar el semáforo 0");
		free(array);
		return ERROR;
	}	

	printf("Up semaforo 0\n");
	fflush(stdout);

	ret = DownMultiple_Semaforo(sem_id_binario,N_SEMAFOROS,SEM_UNDO,num_sem);

	if(ret == ERROR){
		perror("Error al decrementar el array de semáforos");
		free(array);
		return ERROR;
	}

	else if(ret == ERROR_SIN_REESTABLECER){
		perror("Error, no controlamos el estado de los semáforos. Al hacer DownMultiple_Semaforo");
		free(array);
		return ERROR;	
	}

	printf("DownMultiple_Semaforo\n");
	fflush(stdout);

	ret = UpMultiple_Semaforo(sem_id_binario,N_SEMAFOROS,SEM_UNDO,num_sem); 
	
	if(ret == ERROR){
		perror("Error al aumentar el array de semáforos");
		free(array);
		return ERROR;
	}

	else if(ret == ERROR_SIN_REESTABLECER){
		perror("Error, no controlamos el estado de los semáforos. Al hacer UpMultiple_Semaforo");
		free(array);
		return ERROR;	
	}

	printf("UpMultiple_Semaforo\n");
	fflush(stdout);

	if(Borrar_Semaforo(sem_id_binario) == ERROR){
		perror("Error al borrar el semáforo");
		free(array);
		return ERROR;
	}
	
	printf("Borrar_Semaforo\n");
	printf("Ejecución correcta\n");
	return OK;
	
}
