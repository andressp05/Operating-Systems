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


char* getPrograma(char* path){
    char* prog;
    char* ret;
    char ch = '/';
    
    ret = strrchr(path, ch);
    if(ret == NULL)
        return NULL;
    
    prog = strchr(ret, ret[1]);
    
    return prog;
}


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
        wait(NULL);
        
		if (fpid == 0){
            if(strcmp("-l", argv[argc-1]) == 0){
                char* prog;
                prog = getPrograma(argv[i]);
                if(prog == NULL){
                    printf("Se debe introducir la ruta del programa\n");
                    return EXIT_FAILURE;
                }
            	execl(argv[i], prog, NULL);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            }
            else if(strcmp("-lp", argv[argc-1]) == 0){
            	execlp(argv[i], argv[i], NULL);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            }
            else if(strcmp("-v", argv[argc-1]) == 0){
                char* aux[2];
                aux[0] = getPrograma(argv[i]);
                if(aux[0] == NULL){
                    printf("Se debe introducir la ruta del programa\n");
                    return EXIT_FAILURE;
                }
                aux[1] = NULL;
            	execv(argv[i], aux);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            }
            else if(strcmp("-vp", argv[argc-1]) == 0){
                char *aux[2];
            	aux[0] = argv[i];
            	aux[1] = NULL;
            	execvp(argv[i], aux);
            	printf("Programa %d no ha funcionado\n", i);
            	return EXIT_FAILURE;
            }
            else{
                printf("El último parámetro no es válido\n");
                exit(EXIT_FAILURE);
            }
        }
	}
	exit(EXIT_SUCCESS);
}