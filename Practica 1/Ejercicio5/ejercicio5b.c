/**
 * @brief Implementa el ejercicio 5b de procesos con un solo padre
 * @file ejercicio5b.c
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
* @brief funcion de procesos con un solo padre
* @return int: valor de exito o fracaso
*/
int main (void){
    int fpid = 1;
    int pid;
    int ppid;
    int i;
    
    for(i=0; i < NUM_PROC; i++){
        if (fpid == 0){
            pid = getpid();
            ppid = getppid();
            printf("HIJO %d: PID PADRE: %d\t PID HIJO: %d\n", i, ppid, pid);
        } else{
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            pid = getpid();
            printf ("PADRE %d: PID %d \n", i, pid);
        }
    }
    wait(NULL);
    getchar();
    exit(EXIT_SUCCESS);
} 