/**
 * @brief Implementa el ejercicio 4b de multiplicacion de matrices
 * @file ejercicio4b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 04/03/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/*tamaño maximo a leer por pantalla*/
#define MAXTAM 256

/**
 * @brief Estructura params
 */
typedef struct _params{
    int dim;/*!<Dimension de la matriz*/
    int multiplicador;/*!<escalar*/
    int* matriz;/*!<Matriz*/
    int nhilo;/*!<numero de hilo*/
    int fila;/*!<numero de fila*/
    struct _params* otroHilo;/*!<<comunicacion otro hilo*/
}params;

/**
* @brief funcion que multiplica una matriz por un escalar
* @param arg se le pasará una estructura de hilos
* @return void*: finaliza la funcion con la salida del hilo
*/
void* multiplicacion(void* arg){
    int i, j;
    int dim;
    int mult;
    int nhilo;
    int fila;
    int* matriz;
    params* datos;
    
    datos = (params*) arg;
    dim = datos->dim;
    mult = datos->multiplicador;
    matriz = datos->matriz;
    nhilo = datos->nhilo;
    
    for(i = 0; i < dim; i++){
        printf("Hilo %d multiplicando fila %d resultado", nhilo, i);
        for(j = 0; j < dim; j++){
            printf(" %d ", mult*matriz[i*dim+j]);
        }
        datos->fila = i+1;
        printf("- el Hilo %d ", datos->otroHilo->nhilo);
        if((fila = datos->otroHilo->fila) == dim){
            printf("ha acabado\n");
        } else{
            printf("va por la fila %d\n", fila);
        }
        usleep(20000);/*Se duerme para percibir el paralelismo*/
    }
    
    pthread_exit(NULL);
}

/**
* @brief funcion que implementa la multiplicacion de matriz por escalar
* mediante dos hilos 
* @param np numero de primos a calcular 
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
int main(int argc, char* argv[]){
    int i;
    int dim;
    int mult1, mult2;
    int* matriz1;
    int* matriz2;
    params* hilo1;
    params* hilo2;
    char aux[MAXTAM];
    char* delim = " ";
    char* token;
    pthread_t th1, th2;
    
    /*Introduccion de parametros por pantalla*/
    printf("Introduzca dimension de la matriz cuadrada:\n");
    scanf("%d", &dim);
    
    /*Condicion de error pedida*/
    while(dim <= 0 || dim > 4){
        printf("La dimensión ha de ser un entero positivo menor que 4:\n");
        scanf("%d", &dim);
    }
    
    printf("Introduzca multiplicador 1:\n");
    scanf("%d", &mult1);
    
    printf("Introduzca multiplicador 2:\n");
    scanf("%d", &mult2);
    
    matriz1 = (int*) malloc(sizeof(int)*(dim*dim));
    if(matriz1 == NULL){
        printf("Error reservando memoria para la primera matriz\n");
        return EXIT_FAILURE;
    }
    
    matriz2 = (int*) malloc(sizeof(int)*(dim*dim));
    if(matriz2 == NULL){
        printf("Error reservando memoria para la segunda matriz\n");
        free(matriz1);
        return EXIT_FAILURE;
    }
    
    hilo1 = (params*) malloc(sizeof(params));
    if(hilo1 == NULL){
        printf("Error reservando memoria para la estructura de parametros del primer hilo\n");
        free(matriz1);
        free(matriz2);
        return EXIT_FAILURE;
    }
    
    hilo2 = (params*) malloc(sizeof(params));
    if(hilo2 == NULL){
        printf("Error reservando memoria para la estructura de parametros del segundo hilo\n");
        free(matriz1);
        free(matriz2);
        free(hilo1);
        return EXIT_FAILURE;
    }

    printf("Introduzca matriz 1:\n");
    fgets(aux, MAXTAM, stdin);
    fgets(aux, MAXTAM, stdin);
    
    token = strtok(aux, delim);
    for(i = 0; i < dim*dim && token != NULL; i++){
        matriz1[i] = atoi(token);
        token = strtok(NULL, delim);
    }
    
    while(i < dim*dim){
        printf("No se han introducido suficientes números para la primera matriz. ");
        printf("Vuelva a intentarlo:\n");
        fgets(aux, MAXTAM, stdin);
        
        token = strtok(aux, delim);
        
        for(i = 0; i < dim*dim && token != NULL; i++){
            matriz1[i] = atoi(token);
            token = strtok(NULL, delim);
        }
    }
    
    printf("Introduzca matriz 2:\n");
    fgets(aux, MAXTAM, stdin);
    
    token = strtok(aux, delim);
    for(i = 0; i < dim*dim && token != NULL; i++){
        matriz2[i] = atoi(token);
        token = strtok(NULL, delim);
    }
    
    while(i < dim*dim){
        printf("No se han introducido suficientes números para la segunda matriz. ");
        printf("Vuelva a intentarlo:\n");
        fgets(aux, MAXTAM, stdin);
        
        token = strtok(aux, delim);
        
        for(i = 0; i < dim*dim && token != NULL; i++){
            matriz2[i] = atoi(token);
            token = strtok(NULL, delim);
        }
    }
    
    /*Asignacion a los hilos de los valores pedidos por pantalla*/
    hilo1->dim = dim;
    hilo1->multiplicador = mult1;
    hilo1->matriz = matriz1;
    hilo1->nhilo = 1;
    hilo1->fila = 0;
    hilo1->otroHilo = hilo2;
    
    hilo2->dim = dim;
    hilo2->multiplicador = mult2;
    hilo2->matriz = matriz2;
    hilo2->nhilo = 2;
    hilo2->fila = 0;
    hilo2->otroHilo = hilo1;
    
    /*Trabajo con hilos*/
    pthread_create(&th1, NULL, multiplicacion, (void*) hilo1);
    pthread_create(&th2, NULL, multiplicacion, (void*) hilo2);
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    
    pthread_cancel(th1);
    pthread_cancel(th2);
    
    exit(EXIT_SUCCESS);
}