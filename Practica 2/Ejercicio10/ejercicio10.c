/**
 * @brief Implementa el ejercicio 10 de mascaras
 * @file ejercicio10.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 11/03/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


/**
* @brief funcion de procesos con un solo padre
* @return int: valor de exito o fracaso
*/
int main (){
    int fpid = 1;
    int i,cont = 0;
    char string[] = "EL PROCESO A ESCRIBE EN UN FICHERO HASTA QUE LEE LA CADENA FIN"
    char* token;
    char* auxA;
    char* auxB;
    FILE *f = NULL;
    fp = fopen("cadena.txt","a");

    token = strtok(string," ");
    /*creacion de procesos*/
    if ((fpid=fork()) == -1){
        printf("Error al emplear fork\n");
        exit(EXIT_FAILURE);
        }
    if(fpid == 0){
        while(1){
            aux = token[rand()%strlen(token)];
            fprintf(f,"%s",auxA);
            if(strcmp(auxA,"FIN") == 0){
                fclose(f);
                exit(EXIT_SUCCESS);
            }
        }
    }

    else if(fpid > 0){
        while(cont < 50){
            sleep(5);
            fseek(f, 0L, SEEK_CURL);
            fscanf(f,"%s",auxB);
            cont++;
            if(strcmp(auxB,"FIN") == 0){
                fork();
            }
        }
        kill(fpid, SIGKILL);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
} 