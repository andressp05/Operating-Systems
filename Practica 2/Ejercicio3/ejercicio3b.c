/**
 * @brief Implementa el ejercicio 3b de hilos de un proceso
 * @file ejercicio3b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 03/03/2017
 */


#include <time.h>
#include <pthread.h>
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
void *is_prime(int np){
    int* array;
    int i, nprimos = 0, n = 2;

    array = (int*) malloc (sizeof(int)*np);
    if(!array){
        return ;
    }
    printf("mola");
    fflush(stdout);
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

    free(array);
    printf("mazo");
    fflush(stdout);
    pthread_exit(NULL);
}

/**
* @brief funcion de hilos de un proceso
* @param argc contiene el número de parámetros totales pasados
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
int main (int argc, char *argv[]){
    pthread_t h;
    int i;

    if(argc < 1 || argc > 2){
        printf("Se debe pasar un solo parámetro\n");
        return EXIT_FAILURE;
    }
    printf("h");
    fflush(stdout);   
    for(i=0;i<NUM_PROC;i++){
        pthread_create(&h,NULL, is_prime(atoi((argv[1]))),(void *) i);
        printf("si");
        fflush(stdout);
        pthread_join(h,NULL);
    }
    exit(EXIT_SUCCESS);
}