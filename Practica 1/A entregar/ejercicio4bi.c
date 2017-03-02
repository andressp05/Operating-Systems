/**
 * @brief Implementa el ejercicio 4b pedido para analizar sus diferencias con a)
 * @file ejercicio4bi.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version I) Copia del codigo dado para analizar sus diferencias con a)
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
* @brief funcion dada para ser analizada
* @return int: valor de exito o fracaso
*/
int main (void){
   
   int pid;
   int i;
   for (i=0; i < NUM_PROC; i++){
      if ((pid = fork()) < 0 ){
         printf("Error haciendo fork\n");
         exit(EXIT_FAILURE);
      }
       else if (pid == 0){
         printf("HIJO %d\n", i);
       }else{
         printf ("PADRE %d \n", i);
       }
   }
wait(NULL);
exit(EXIT_SUCCESS);

}
