/**
 * @brief Implementa el ejercicio 4a de multiplicacion de matrices
 * @file ejercicio4a.c
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

/**
* @brief Definicion de la macro MAXTAM
*/
#define MAXTAM 256 /*tamanyo maximo*/

/**
 * @brief Estructura params que contiene los parametros necesarios para que el
 * hilo multiplique un matriz por un numero entero
 */

typedef struct _params{
    int dim; /*!< Dimension matriz*/
    int multiplicador; /*!< Escalar*/
    int* matriz; /*!< Matriz*/
    int nhilo; /*!< Identificador del hilo*/
}params;

/**
* @brief funcion que comprueba si la matriz introducida por el usuario
* es valida
* @param aux cadena de caracteres que ha introducido el usuario
* @param dim dimension de la matriz
* @param matriz array donde se almacenaran los enteros introducidos por
* el usuario si la matriz es valida
* @return int: EXIT_SUCCESS en caso de que la matriz es valida o
* EXIT_FAILURE en caso de que no se hayan introducido suficientes numeros
* o haya introducido caracteres no numericos
*/
int comprueba_matriz(char* aux, int dim, int* matriz){
    int i, j = 0;
    char* delim = " ";
    char* token;
    
    while(aux[j] != 0 && aux[j] != 10){
        if((aux[j] < '0' || aux[j] > '9') && aux[j] != ' ' && aux[j] != '-' && aux[j] != '+'){
            return EXIT_FAILURE;
        }
        j++;
    }
    
    token = strtok(aux, delim);
    for(i = 0; i < dim*dim && token != NULL; i++){
        matriz[i] = atoi(token);
        token = strtok(NULL, delim);
    }
    
    if(i < dim*dim){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
* @brief funcion que multiplica una matriz por un escalar
* @param arg se le pasara la estructura de hilos
* @return void*: finaliza la funcion con la salida del hilo
*/
void* multiplicacion(void* arg){
    int i, j;
    int dim;
    int mult;
    int nhilo;
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
            printf(" %d", mult*matriz[i*dim+j]);
        }
        printf("\n");
        fflush(stdout);
        usleep(20000);
    }
    return NULL;    
}

/**
* @brief funcion que implementa la multiplicacion de matriz por escalar
* mediante dos hilos 
* @return int: valor de exito o fracaso
*/
int main(){
    int ret;
    int dim;
    int mult1, mult2;
    int* matriz1;
    int* matriz2;
    params* hilo1;
    params* hilo2;
    char aux[MAXTAM];
    pthread_t th1, th2;
    
    printf("Introduzca dimension de la matriz cuadrada:\n");
    fgets(aux, MAXTAM, stdin);
    dim = atoi(aux);
    
    while(dim <= 0 || dim > 4){
        printf("La dimensión ha de ser un entero positivo menor que 4:\n");
        fgets(aux, MAXTAM, stdin);
        dim = atoi(aux);
    }
    
    printf("Introduzca multiplicador 1:\n");
    fgets(aux, MAXTAM, stdin);
    mult1 = atoi(aux);
    
    while(mult1 == 0){
        printf("No es un multiplicador válido. Vuelva a intentarlo:\n");
        fgets(aux, MAXTAM, stdin);
        mult1 = atoi(aux);
    }
    
    printf("Introduzca multiplicador 2:\n");
    fgets(aux, MAXTAM, stdin);
    mult2 = atoi(aux);
    
    while(mult2 == 0){
        printf("No es un multiplicador válido. Vuelva a intentarlo:\n");
        fgets(aux, MAXTAM, stdin);
        mult2 = atoi(aux);
    }
    
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
    ret = comprueba_matriz(aux, dim, matriz1);
    while(ret == EXIT_FAILURE){
        printf("La matriz introducida no es válida. Compruebe que ha introducido %d números enteros:\n", dim*dim);
        fgets(aux, MAXTAM, stdin);
        ret = comprueba_matriz(aux, dim, matriz1);
    }
    
    printf("Introduzca matriz 2:\n");
    fgets(aux, MAXTAM, stdin);
    ret = comprueba_matriz(aux, dim, matriz2);
    while(ret == EXIT_FAILURE){
        printf("La matriz introducida no es válida. Compruebe que ha introducido %d números enteros:\n", dim*dim);
        fgets(aux, MAXTAM, stdin);
        ret = comprueba_matriz(aux, dim, matriz2);
    }
    
    hilo1->dim = dim;
    hilo1->multiplicador = mult1;
    hilo1->matriz = matriz1;
    hilo1->nhilo = 1;
    
    hilo2->dim = dim;
    hilo2->multiplicador = mult2;
    hilo2->matriz = matriz2;
    hilo2->nhilo = 2;
    
    ret = pthread_create(&th1, NULL, multiplicacion, (void*) hilo1);
    if(ret) {
        printf("Error al crear el primer hilo.\n");
        return EXIT_FAILURE;
    }
    
    ret = pthread_create(&th2, NULL, multiplicacion, (void*) hilo2);
    if(ret) {
        printf("Error al crear el segundo hilo.\n");
        return EXIT_FAILURE;
    }
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    
    pthread_cancel(th1);
    pthread_cancel(th2);
    
    free(hilo1);
    free(hilo2);
    free(matriz1);
    free(matriz2);
    
    exit(EXIT_SUCCESS);
}