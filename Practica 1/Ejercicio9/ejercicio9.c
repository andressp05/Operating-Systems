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
/*!Macro lectura*/
#define LEER 0
/*!Macro escribir*/
#define ESCRIBIR 1

/**
* @brief funcion en la que los hijos hacen las operaciones
* y el padre mostrara el mensaje. Se realiza una llamada a
* la funcion wait para que el padre espere a cada hijo.
* @return int: valor de exito o fracaso
*/
int main (int argc,char* argv[]){
    int i;
    int parent_pipe[2], child_pipe[2], nbytes, pipe_status;
    pid_t childpid;
    char readbuffer[200];
    char writebuffer[200];
    int op1,op2;
    char string[100];

    printf("Introduce el primer numero: ");
    scanf("%d", &op1);
    printf("Introduce el segundo numero: ");
    scanf("%d", &op2);

    for (i=0; i < NUM_PROC; i++){
        /* Se limpian los dos buffers para no sobreescribir e imprimir basura */
        memset(readbuffer, 0, 200);
        memset(writebuffer, 0, 200);
        
        /* Se crea la tuberia por donde el padre escribira */
        pipe_status = pipe(parent_pipe);
        if(pipe_status == -1){
	    perror("Error creando la primera tuberia\n");
            exit(EXIT_FAILURE);
        }
        
        /*Se crea la tuberia por donde el hijo escribira */ 
        pipe_status = pipe(child_pipe);
        if(pipe_status == -1){
	    perror("Error creando la segunda tuberia\n");
            exit(EXIT_FAILURE);
        }
        
        if ((childpid = fork()) == -1){
            perror("Error al emplear fork\n");
            exit(EXIT_FAILURE);
        }
        
        if (childpid > 0){
            /*El padre escribe en la primera tuberia los datos introducidos por el usuario */
            close(parent_pipe[LEER]);
	        sprintf(string,"%d,%d",op1,op2);
    	    write(parent_pipe[ESCRIBIR],string,strlen(string));
    	    
    	    /*El padre espera a que termine el hijo y lee e imprime por pantalla lo que ha escrito el hijo en la segunda tuberia */
	        wait(NULL);
	        close(child_pipe[ESCRIBIR]);
	        nbytes = read(child_pipe[LEER],readbuffer,sizeof(readbuffer));
	        if(nbytes <= 0){
	            printf("Error al leer el retorno del hijo\n");
	        } else printf("%s", readbuffer);
        } else{
            /*El hijo lee lo que ha escrito el padre en la primera tuberia y escribe la informacion en la segunda tuberia*/
            close(parent_pipe[ESCRIBIR]);
		    nbytes = read(parent_pipe[LEER],readbuffer,sizeof(readbuffer));
		    if(nbytes <= 0){
		        printf("Error al leer los operandos de la tubería\n");
		        exit(EXIT_FAILURE);
		    }
		    
		    sscanf(readbuffer,"%d,%d",&op1,&op2);
		    
		    if(i==0){
		        close(child_pipe[LEER]);
			    sprintf(writebuffer, "Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. Suma: %d.\n", getpid(), op1, op2, op1+op2);
    			write(child_pipe[ESCRIBIR], writebuffer, strlen(writebuffer));
            } else if(i==1){
    		    close(child_pipe[LEER]);
        		sprintf(writebuffer, "Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. Resta: %d.\n", getpid(), op1, op2, op1-op2);
    			write(child_pipe[ESCRIBIR], writebuffer, strlen(writebuffer));
            } else if(i==2){
    		    close(child_pipe[LEER]);
    			sprintf(writebuffer, "Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. Multiplicacion: %d.\n", getpid(), op1, op2, op1*op2);
    			write(child_pipe[ESCRIBIR], writebuffer, strlen(writebuffer));
            } else{
    		    close(child_pipe[LEER]);
    		    /*Se comprueba que el segundo operando no sea nulo, dado que no se puede dividir por cero*/
    		    if(op2 == 0){
        	        sprintf(writebuffer, "Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. División: No se puede dividir por cero\n", getpid(), op1, op2);
    		    } else{
                    sprintf(writebuffer, "Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. Division: %d.\n", getpid(), op1, op2, op1/op2);
    		    }
    		    write(child_pipe[ESCRIBIR], writebuffer, strlen(writebuffer));
            }
	        exit(EXIT_SUCCESS);
	    }
    }
    exit(EXIT_SUCCESS);
}