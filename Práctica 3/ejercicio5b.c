/**
 * @brief Implementa el ejercicio 5b de semaforos (test con N-ARIOS)
 * @file ejercicio5b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 05/03/2017
 */
#include "semaforos.h"
#include <sys/sem.h>


/**
* @brief Define la clave precompartida de los semaforos
*/
#define SEMKEY 75798

/**
* @brief Define el numero total de semaforos
*/
#define N_SEMAFOROS 3


/**
* @brief funcion principal/ test con semaforos BINARIOS y N-ARIOS
*   siguiendo el codigo se explica que se va realizando
* @return int: valor de exito (OK) o fracaso (ERROR)
*/
int main (){
    /*
    * Declaración de variables
    */
    int sem_id; /* IDs de semaforos*/
    int ret; /*variable para guardar el retorno de los multiple down y up*/
    int i; /*variable para inicializar todos los valores de los semaforos*/
    unsigned short *array; /* array de semaforos*/
    int num_sem[] = {0, 1, 2}; /* array estatico para los tres semaforos*/

    /* Reservamos memoria al array */
    array = (unsigned short *)malloc(sizeof(short)*N_SEMAFOROS);
    if(!array){
        perror("Error al reservar memoria");
        return ERROR;
    }

    /*
    * Inicializamos el array de semaforos binarios
    */
    for(i = 0; i < N_SEMAFOROS; i++){
        array[i] = i+1;
    }   

    /*
    * Creamos una lista o conjunto con tres semáforos
    */
    if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id) != 0){
        perror("Error al crear semáforos");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Crear\n");
    fflush(stdout);

    /*
    * Vemos que los semáforos ya se han creado
    */
    if(Crear_Semaforo(SEMKEY, N_SEMAFOROS, &sem_id) != 1){
        perror("Error al intentar crear semáforos ya existentes");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Crear con semaforos ya existentes\n");
    fflush(stdout);

    /*
    * Inicializamos los semáforos
    */
    if(Inicializar_Semaforo(sem_id, array) == ERROR){
        perror("Error al inicializar semáforos");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Inicializar\n");
    fflush(stdout);

    /*
    * Operamos sobre los semáforos
    */

    /* Realizamos el down multiple */
    ret = DownMultiple_Semaforo(sem_id,N_SEMAFOROS,SEM_UNDO,num_sem);

    if(ret == ERROR){
        perror("Error al decrementar el array de semáforos");
        free(array);
        return ERROR;
    }

    else if(ret == ERROR_SIN_REESTABLECER){
        perror("Error, no controlamos el estado de los semáforos. Al hacer DownMultiple_Semaforo");
        free(array);
        return ERROR;   
    }

    /* Comprueba que funciona correctamente*/
    printf("DownMultiple_Semaforo\n");
    fflush(stdout);

    /* Realizamos un down del segundo semaforo */
    if(Down_Semaforo(sem_id, 1, SEM_UNDO) == ERROR){
        perror("Error al decrementar el semáforo 1");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Down semaforo 1\n");
    fflush(stdout);

    /* Realizamos un down del tercer semaforo */
    if(Down_Semaforo(sem_id, 2, SEM_UNDO) == ERROR){
        perror("Error al decrementar el semáforo 2");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Down semaforo 2\n");
    fflush(stdout);

    /* Realizamos un nuevo down del tercer semaforo */
    if(Down_Semaforo(sem_id, 2, SEM_UNDO) == ERROR){
        perror("Error al decrementar el semáforo 2");
        free(array);
        return ERROR;
    }

    /* Comprueba que funciona correctamente*/
    printf("Down semaforo 2\n");
    fflush(stdout);

    /* Realizamos un up del tercer semaforo*/
    if(Up_Semaforo(sem_id, 2, SEM_UNDO) == ERROR){
        perror("Error al aumentar el semáforo 2");
        free(array);
        return ERROR;
    }   

    /* Comprueba que funciona correctamente*/
    printf("Up semaforo 2\n");
    fflush(stdout);

    /* Realizamos un nuevo up del tercer semaforo*/
    if(Up_Semaforo(sem_id, 2, SEM_UNDO) == ERROR){
        perror("Error al aumentar el semáforo 2");
        free(array);
        return ERROR;
    }   

    /* Comprueba que funciona correctamente*/
    printf("Up semaforo 2\n");
    fflush(stdout);

    /* Realizamos un up del segundo semaforo*/
    if(Up_Semaforo(sem_id, 1, SEM_UNDO) == ERROR){
        perror("Error al aumentar el semáforo 1");
        free(array);
        return ERROR;
    }   

    /* Comprueba que funciona correctamente*/
    printf("Up semaforo 1\n");
    fflush(stdout);

    /* Realizamos el up multiple */
    ret = UpMultiple_Semaforo(sem_id,N_SEMAFOROS,SEM_UNDO,num_sem); 
    
    if(ret == ERROR){
        perror("Error al aumentar el array de semáforos");
        free(array);
        return ERROR;
    }

    else if(ret == ERROR_SIN_REESTABLECER){
        perror("Error, no controlamos el estado de los semáforos. Al hacer UpMultiple_Semaforo");
        free(array);
        return ERROR;   
    }

    /* Comprueba que funciona correctamente*/
    printf("UpMultiple_Semaforo\n");
    fflush(stdout);

    /*
    * Borramos los semaforos
    */
    if(Borrar_Semaforo(sem_id) == ERROR){
        perror("Error al borrar el semáforo");
        free(array);
        return ERROR;
    }
    
    /* Comprueba que funciona correctamente*/
    printf("Borrar_Semaforo\n");
    fflush(stdout);

    free(array);
    
    /* Comprueba que la ejecucion ha sido buena*/
    printf("Ejecución correcta\n");
    return OK;
    
}
