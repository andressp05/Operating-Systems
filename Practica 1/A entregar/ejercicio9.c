/**
 * @brief Implementa el ejercicio 9 de tuberias
 * @file ejercicio9.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 10/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*!Numero de hijos a crear*/
#define NUM_PROC 4

/**
* @brief funcion en la que los hijos hacen las operaciones
* y el padre mostrara el mensaje
* @return int: valor de exito o fracaso
*/
int main (int argc,char* argv[]){
    int i;
    int fd[2],nbytes,pipe_status;
    pid_t childpid;
    char readbuffer[150];
    int op1,op2;
    char string[100];

    printf("Introduce el primer numero: ");
    scanf("%d", &op1);
    printf("Introduce el segundo numero: ");
    scanf("%d", &op2);

    for (i=0; i < NUM_PROC; i++){
        pipe_status = pipe(fd);
        if(pipe_status == -1){
	    perror("Error creando la tuberia\n");
            exit(EXIT_FAILURE);
        }
        if ((childpid = fork()) == -1){
            perror("Error al emplear fork\n");
            exit(EXIT_FAILURE);
        }
        if (childpid == 0){
		nbytes = read(fd[0],readbuffer,sizeof(readbuffer));
		sscanf(readbuffer,"%d,%d",&op1,&op2);
		if(i==0){
			printf("Datos enviados a través de la tubería por el proceso PID=%d. ",getpid());
			printf("Operando 1: %d. ",op1);
			printf("Operando 2: %d. ",op2);
			printf("Suma: %d.\n",op1+op2);
        }
		else if(i==1){
			printf("Datos enviados a través de la tubería por el proceso PID=%d. ",getpid());
			printf("Operando 1: %d. ",op1);
			printf("Operando 2: %d. ",op2);
			printf("Resta: %d.\n",op1-op2);
        }
		else if(i==2){
			printf("Datos enviados a través de la tubería por el proceso PID=%d. ",getpid());
			printf("Operando 1: %d. ",op1);
			printf("Operando 2: %d. ",op2);
			printf("Multiplicacion: %d.\n",op1*op2);
        }
		else{
			printf("Datos enviados a través de la tubería por el proceso PID=%d. ",getpid());
			printf("Operando 1: %d. ",op1);
			printf("Operando 2: %d. ",op2);
			printf("Division: %d.\n",op1/op2);
        }
	exit(EXIT_SUCCESS);
	}
        else{
	    sprintf(string,"%d,%d",op1,op2);
	    write(fd[1],string,strlen(string));
	    wait(NULL);
        }
    }    exit(EXIT_SUCCESS);
}
