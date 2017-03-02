/**
 * @brief Implementa el ejercicio 8 de ejecucion
 * @file ejercicio8.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 17/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* @brief funcion que crea un cierto número de procesos hijos y cada uno ejecuta
* un programa de los pasados por argumento de entrada. Se realiza una llamada a
* la funcion wait para que espere a la finalizacion de cada proceso hijo
* @param argc contiene el número de parámetros totales pasados
* @param argv[] debe contener la ruta de los programas que ejecutarán los hijos
* seguido de -l o -v para utilizar execl o execv o debe contener los programas
* a ejecutar seguido de -lp o -vp para utilizar execlp o execvp
* @return int: valor de exito o fracaso
*/
int main(int argc, char* argv[]){
	int i;
	int fpid;
	
	if(argc < 3){
	    printf("El número de argumentos introducidos no es suficiente\n");
	    exit(EXIT_FAILURE);
	}

	for(i = 1; i < argc - 1; i++){
	    if ((fpid = fork()) < 0){
            printf("Error al emplear fork\n");
            exit(EXIT_FAILURE);
        }
        
        /* llamada a la funcion wait para que el padre espere a todos sus hijos */
        wait(NULL);
        
		if (fpid == 0){
            if(strcmp("-l", argv[argc-1]) == 0){
                /* llamada a execl con la ruta del comando a ejecutar en argv[i] */
            	execl(argv[i], argv[i], NULL);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            } else if(strcmp("-lp", argv[argc-1]) == 0){
                /* llamada a execlp con el comando a ejecutar en argv[i] */
            	execlp(argv[i], argv[i], NULL);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            } else if(strcmp("-v", argv[argc-1]) == 0){
                char* aux[2];
                aux[0] = argv[i];
                aux[1] = NULL;
                /* llamada a execv con la ruta del comando a ejecutar en argv[i] y en aux[0] */
            	execv(argv[i], aux);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            } else if(strcmp("-vp", argv[argc-1]) == 0){
                char* aux[2];
            	aux[0] = argv[i];
            	aux[1] = NULL;
            	/*llamada a execvp con el comando a ejecutar en argv[i] y en aux[0]*/
            	execvp(argv[i], aux);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            } else{
                printf("El último parámetro no es válido\n");
                exit(EXIT_FAILURE);
            }
        }
	}
	exit(EXIT_SUCCESS);
}