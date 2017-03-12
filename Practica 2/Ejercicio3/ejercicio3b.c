/**
 * @brief Implementa el ejercicio 3b de hilos de un proceso
 * @file ejercicio3b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 03/03/2017
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>/*calculo tiempo*/
#include <unistd.h>

/*!Numero de hilos del bucle for*/
#define NUM_HIL 100

/**
* @brief funcion que lista los np primeros primos 
* @param np numero de primos a calcular 
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
void* is_prime(void* arg){
    int* array;
    int i, nprimos = 0, n = 2, np = *((int*) arg);

    array = (int*) malloc (sizeof(int)*np);
    if(!array){
        return NULL;
    }

    while(nprimos <= np){
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

        for(i=3;i*i<=n;i+=2){/*No miramos los pares*/
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

    return NULL;
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
    int n;
    int ret;
    /*variables necesarias calculo tiempo*/
    struct timeval ti, tf;
    double tiempo;

    if(argc != 2){
        printf("Se debe pasar un solo parámetro\n");
        return EXIT_FAILURE;
    }
    
    gettimeofday(&ti, NULL);
    n = atoi(argv[1]);
    
    /*creacion de hilos*/
    for(i = 0; i < NUM_HIL; i++){
        ret = pthread_create(&h, NULL, is_prime, (void*) &n);
        if(ret){
            printf("Error al crear el hilo %d\n", i+1);
            exit(EXIT_FAILURE);
        }
        pthread_join(h,NULL);
        pthread_cancel(h);
    }
    
    gettimeofday(&tf, NULL);
    /*realizamos el calculo del tiempo, primero los segundos y despues los microsegundos*/
    tiempo = (tf.tv_sec - ti.tv_sec) + (tf.tv_usec - ti.tv_usec)/1000000.0;
    printf("Tiempo de ejecucion: %gs\n", tiempo);

    exit(EXIT_SUCCESS);
}
