/**
 * @brief Implementa el ejercicio cadena_montaje.c de mensajes
 * @file cadena_montaje.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 8/04/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>

/**
* @brief Definicion de la clave
*/
#define KEY 1300

/**
* @brief Definicion de la clave de fichero
*/
#define FILEKEY "/bin/cat"

/**
* @brief Definicion del numero de procesos hijos totales
*/
#define NUM_PROCESOS 3

/**
* @brief Definicion del tamanyo del "trozo" de lectura del fichero
*/
#define TROZOS 5


/**
 * @brief Estructura mensaje que contiene todos sus parametros necesarios para la
 * realización del ejercicio con colas de mensajes
 */
typedef struct _Mensaje{
	long id; /*!<Tipo de mensaje*/
	/*Informacion a transmitir en el mensaje*/
	int valor; 
	char aviso[80];
} mensaje;

/**
* @brief funcion principal que pide un fichero de entrada y otro
*   de salida y realiza el paso de mensajes pedidos
* @param argc: contiene el número de parámetros totales pasados
* @param argv: contiene los parámetros pasados por el usuario
* @return int: valor de exito (OK) o fracaso (ERROR)
*/
int main(int argc, char* argv[]){
    /*
    * Declaración de variables
    */
    char *origen;
    char *destino;
    char aux[TROZOS];
    char aux2[TROZOS];
    int i,j;
    int pid = 1;
    int msqid;
	mensaje msg;
    key_t key;
    FILE* fo = NULL;
    FILE* fd = NULL;
    
    /*
    * Comprobacion de errores
    */
    if(argc != 3){
        printf("Se debe pasar dos argumentos que seran fichero de origen y de destino\n");
        exit(EXIT_FAILURE);
    }
    
    origen = argv[1];
	destino = argv[2];
    
    printf("Paso 1");

    for(i = 0; i < NUM_PROCESOS; i++){
        if(pid > 0){
            if(i == 0){
                /*  Obtenemos la clave para poder crear la cola de mensajes */
                key = ftok(FILEKEY, KEY);
                if(key == -1){
                    perror("Error al obtener key\n");
                    exit(EXIT_FAILURE);
                }
                
                printf("%d\n", key);

                /* Creacion de la cola de mensajes */
                msqid = msgget(key, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
                if(msqid == -1){
                    msqid = msgget(key, IPC_CREAT | SHM_R | SHM_W);
                    if(msqid == -1){
                        perror("Error al crear la cola de mensajes\n");
                        exit(EXIT_FAILURE);
                    }
                }

                printf("%d\n", msqid);
            }
            
            if((pid = fork()) == -1){
                printf("Error en el fork\n");
                msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
				exit(EXIT_FAILURE);
            }
        } 
        
        /* If para lo que hará el proceso hijo A*/
        if(pid == 0 && i == 0){
        	fo = fopen(origen, "r");
        	if(fo == NULL){
        		printf("Error al abrir el fichero.\n");
        		msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
        		exit(EXIT_FAILURE);
    		}

    		fscanf(fo, "%s", aux);
    		msg.id = 1; /*Tipo de mensaje*/
			msg.valor= 0;
			strcpy (msg.aviso, aux);
			printf("%s\n", msg.aviso);
			msgsnd (msqid, (struct msgbuf *) &msg, sizeof(mensaje) - sizeof(long), IPC_NOWAIT);
    		fclose(fo);
    	}

    	/* Else If para lo que hará el proceso hijo B*/
    	else if(pid == 0 && i == 1){
    		msgrcv (msqid, (struct msgbuf *) &msg, sizeof(mensaje) - sizeof(long), 1, 0);
    		
    		printf("%s\n", msg.aviso);
			
    		for(j=0;msg.aviso[j] != '\0';j++){
    			if(msg.aviso[j] < 'a' || msg.aviso[j] > 'z')
    				continue;
    			msg.aviso[j] = (int) msg.aviso[j] - 32;
    			printf("%d %s\n", j,msg.aviso);
    		}
			
			printf("%s\n", msg.aviso);
    		msg.id = 2;
    		msgsnd (msqid, (struct msgbuf *) &msg, sizeof(mensaje) - sizeof(long), IPC_NOWAIT);
    	}

    	/* Else If para lo que hará el proceso hijo C*/
    	else if(pid == 0 && i == 2){
			fd = fopen(destino, "a");
        	if(fd == NULL){
        		printf("Error al abrir el fichero.\n");
        		msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
        		exit(EXIT_FAILURE);
    		}

    		msgrcv (msqid, (struct msgbuf *) &msg, sizeof(mensaje) - sizeof(long), 2, 0);
			
			printf("%s\n", msg.aviso);
    		
			for(j=0;msg.aviso[j] != '\0';j++){
				aux2[j] = msg.aviso[j];  
			}
			
			fprintf(fd, "%s", aux2);
			
			fclose(fd);		
    	}
	}
	msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
    exit(EXIT_SUCCESS);
}
