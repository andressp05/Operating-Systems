/**
 * @brief Implementa el ejercicio 4b con los cambios pedidos
 * @file ejercicio4bii.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version II) Cambios en el codigo dado.
 * @date 03/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*!Numero de procesos del bucle for dado*/
#define NUM_PROC 3

/**
* @brief funcion en la que el hijo imprime su pid 
* y el del padre
* @return int: valor de exito o fracaso
*/
int main (void){
   int fpid;
   int ppid;
   int pid;
   int i;
   for (i=0; i < NUM_PROC; i++){
      if ((fpid = fork()) < 0 ){
         printf("Error al emplear fork\n");
         exit(EXIT_FAILURE);
      }
       else if (fpid == 0){
         pid = getpid();   /* Uso de la funcion para obtener pid actual*/
         ppid = getppid(); /* Uso de la funcion para obtener pid Padre*/
         printf("PADRE %d del HIJO %d\n", ppid, pid);
       }else{
         pid = getpid();/* Uso de la funcion para obtener pid actual*/
         printf("PADRE %d\n", pid);
       }getchar();/* Usado para poder usar ps -auxf por terminal*/
   }
wait(NULL);
exit(EXIT_SUCCESS);

}