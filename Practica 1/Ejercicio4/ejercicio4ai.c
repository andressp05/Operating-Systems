/**
 * @brief Implementa el ejercicio 4a pedido para analizar su arbol de procesos
 * @file ejercicio4ai.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version I) Copia del codigo dado para analizar su arbol
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
         printf("Error al emplear fork\n");
         exit(EXIT_FAILURE);
      } else if (pid == 0){
         printf("HIJO %d\n", i);
      } else{
         printf ("PADRE %d \n", i);
      }
   }
   exit(EXIT_SUCCESS);

}
