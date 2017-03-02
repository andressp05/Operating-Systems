/**
 * @brief Implementa el ejercicio 5a de procesos secuenciales
 * @file ejercicio5a.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 10/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*!Numero de procesos del bucle for dado*/
#define NUM_PROC 3

/**
* @brief funcion de procesos secuenciales
* @return int: valor de exito o fracaso
*/
int main (void){
    int fpid = 0;
    int pid;
    int ppid;
    int i;
    
    for(i=0; i < NUM_PROC; i++){
        if(fpid ==0){
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            pid = getpid();
            ppid = getppid();
            printf("PID PADRE: %d\t PID HIJO: %d\n", ppid, pid);
		} else{
            pid = getpid();
            printf ("PID PADRE %d \n", pid);
        }
    }
    wait(NULL);
    getchar();
    exit(EXIT_SUCCESS);
} 