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
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
* @brief Definicion de la macro NUMPALABRAS
*/
#define NUMPALABRAS 13 /*Numero de palabras del fichero*/

/**
* @brief funcion del manejador
* @param sig señal pasada
*/
void captura(int sig){
    return;
}

/**
* @brief funcion de procesos A y B
* @return int: valor de exito o fracaso
*/
int main (){
    int ret = 0;
    int fpid;
    int i = 0;
    int cont = 0;
    int aux;
    char string[] = "EL PROCESO A ESCRIBE EN UN FICHERO HASTA QUE LEE LA CADENA FIN";
    char* palabras[NUMPALABRAS];
    char* token;
    char auxA[NUMPALABRAS];
    sigset_t set, oset, mask;
    FILE* fp = NULL;
    FILE* fh = NULL;
    
    fp = fopen("cadena.txt", "w");
    if(fp == NULL){
        printf("Error al abrir el fichero.\n");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    
    token = strtok(string, " ");
    while(token != NULL){
        palabras[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    
    /*creacion de procesos*/
    if ((fpid=fork()) == -1){
        printf("Error al emplear fork\n");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        if(fpid == 0){
            if(sigfillset(&set) == -1){
                printf("Error al bloquear las señales.\n");
                exit(EXIT_FAILURE);
            }
            if(sigdelset(&set, SIGUSR2) == -1){
                printf("Error al desenmascarar la señal SIGUSR2.\n");
                exit(EXIT_FAILURE);
            }
            if(sigprocmask(SIG_SETMASK, &set, &oset) == -1){
                printf("Error al enmascarar las señales.\n");
                exit(EXIT_FAILURE);
            }
            while(1){
                fh = fopen("cadena.txt", "a");
                if(fh == NULL){
                    printf("Error al abrir el fichero.\n");
                    exit(EXIT_FAILURE);
                }
                aux = rand()%NUMPALABRAS;
                fprintf(fh, "%s ", palabras[aux]);
                printf("\nHIJO %d escribe %s: \n", getpid(), palabras[aux]);
                fflush(stdout);
                fclose(fh);
                kill(getppid(), SIGUSR1);
                if(strcmp(palabras[aux], "FIN") == 0){
                    exit(EXIT_SUCCESS);
                }
                if(signal(SIGUSR2, captura) == SIG_ERR){
                    printf("Error en la señal SIGUSR2\n");
                    exit(EXIT_FAILURE);
                }
                sigsuspend(&set);
            }
        }

        else if(fpid > 0){
            if(sigfillset(&mask) == -1){
                printf("Error al enmascarar las señales.\n");
                exit(EXIT_FAILURE);
            }
            if(sigdelset(&mask, SIGUSR1) == -1){
                printf("Error al desenmascarar la señal SIGUSR1.\n");
                exit(EXIT_FAILURE);
            }
            while(cont < 50){
                if(signal(SIGUSR1, captura) == SIG_ERR){
                    printf("Error en la señal SIGUSR1\n");
                    exit(EXIT_FAILURE);
                }
                sigsuspend(&mask);
                sleep(5);
                fp = fopen("cadena.txt", "r");
                if(fp == NULL){
                    printf("Error al abrir el fichero.\n");
                    exit(EXIT_FAILURE);
                }
                fseek(fp, ret, SEEK_SET);
                fscanf(fp, "%s", auxA);
                printf("Padre lee cadena %d: %s \n", cont+1, auxA);
                fflush(stdout);
                ret = ftell(fp);
                cont++;
                fclose(fp);
                if(strncmp(auxA, "FIN", 3) == 0){
                    wait(NULL);
                    srand(time(NULL));
                    if ((fpid=fork()) == -1){
                        printf("Error al emplear fork\n");
                        exit(EXIT_FAILURE);
                    }
                    if(fpid == 0){
                        break;
                    }
                } else {
                    kill(fpid, SIGUSR2);
                }
            }
            
            if(fpid > 0){
                kill(fpid, SIGKILL);
                wait(NULL);
                exit(EXIT_SUCCESS);
            }
        }
    }
    exit(EXIT_SUCCESS);
} 
