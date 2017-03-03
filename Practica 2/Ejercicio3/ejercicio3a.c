/**
 * @brief Implementa el ejercicio 3a de procesos con un solo padre
 * @file ejercicio3a.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 03/03/2017
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*!Numero de procesos del bucle for dado*/
#define NUM_PROC 10

/**
* @brief funcion que lista los np primeros primos 
* @param np numero de primos a calcular 
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
int* is_prime(int np){
    int* array;
    int i, nprimos = 0, n = 2;

    array = (int*) malloc (sizeof(int)*np);
    if(!array){
        return NULL;
    }

    while(nprimos < np){
        if(n == 2){
            array[nprimos] = n;
            n++;
            nprimos++;
            continue;
        }

        if(n%2 == 0){
            n++;
            continue;
        }

        for(i=3;i*i<n;i+=2){
            if(n%i != 0)
                continue;
            break;
        }

        if(i*i>n){
            array[nprimos] = n;
            nprimos++;
        }
        n++;
    }

    return array;
}

/**
* @brief funcion de procesos con un solo padre
* @param argc contiene el número de parámetros totales pasados
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
int main (int argc, char *argv[]){
    int fpid = 1;
    int i,j;
    int* array;

    if(argc < 1 || argc > 2){
        printf("Se debe pasar un solo parámetro\n");
        return EXIT_FAILURE;
    }

   for(i=0; i < NUM_PROC; i++){
        if (fpid != 0){
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            if(fpid == 0){
                array = is_prime(atoi((argv[1])));
            }            
        }
        wait(NULL);
    }
    getchar();
    free(array);
    exit(EXIT_SUCCESS);
} 
