/**
 * @brief Implementa el ejercicio 6 de relacion entre procesos padre/hijo
 * @file ejercicio6.c
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

/*!Numero de caracteres de la cadena*/
#define CADENA 80

/**
* @brief funcion en la que el proceso padre reserva
* memoria y en el proceso hijo el usuario introduce un nombre
* @return int: valor de exito o fracaso
*/
int main (void){
    int pid;
    char* cad;
    
    cad = (char*)malloc(CADENA*sizeof(char));
    if ((pid=fork()) <0 ){
        printf("Error al emplear fork\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0){
        printf("Introduce una cadena\n");
        fgets(cad, CADENA, stdin);
        printf("\n%s\n", cad);
        free(cad);
        exit(EXIT_SUCCESS);
    } else{
        wait(NULL);
        free(cad);
    }
    exit(EXIT_SUCCESS);
} 


/* El proceso padre no tiene acceso a ese valor ya que el fork
duplica la informacion del padre en el hijo y despues ambos procesos
trabajan en paralelo*/

/*La memoria se libera tanto en el padre tras crear a su hijo como en
el hijo una vez que ha terminado su proceso.*/