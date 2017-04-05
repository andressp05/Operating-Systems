/**
 * @brief Implementa el ejercicio 2 de memoria compartida
 * @file ejercicio2.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 24/03/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>

/**
* @brief Definicion de la clave
*/
#define KEY 1300

/**
* @brief Definicion de la clave de fichero
*/
#define FILEKEY "/bin/cat"

/**
 * @brief Estructura info que contiene los parametros pedidos para la
 * reserva de un bloque de memoria
 */
typedef struct info{
    char nombre[80]; /*!< Nombre del usuario*/
    int id; /*!< Identificador*/
}Info;

/**
* Inicializacion de la estructura a NULL
*/
Info* datos = NULL;

/**
* @brief funcion de captura de señales
* @param signal señal pasada
*/
void capturar(int signal){
    printf("Nombre de usuario: %s\n", (*datos).nombre);
    printf("Identificador: %d\n", (*datos).id);
}

/**
* @brief funcion principal que genera nhijos y que implementa
*   todo lo que debe realizar tanto el padre como sus hijos
* @param argc: contiene el número de parámetros totales pasados
* @param argv: contiene los parámetros pasados por el usuario
* @return int: valor de exito (OK) o fracaso (ERROR)
*/
int main(int argc, char* argv[]){
    /*
    * Declaración de variables
    */
    int i;
    int n;
    int pid = 1;
    int id_zone;
    key_t key;
    
    /*
    * Comprobacion de errores
    */
    if(argc != 2){
        printf("Se debe pasar un número entero positivo como único argumento\n");
        exit(EXIT_FAILURE);
    }
    
    if((n = atoi(argv[1])) <= 0){
        printf("El argumento de entrada debe ser un entero positivo\n");
        exit(EXIT_FAILURE);
    }
    
    /*
    * Realizacion del proceso principal
    */
    for(i = 0; i < n; i++){
        /* Distinguimos entre padre e hijos (aqui lo que realiza el padre) */
        if(pid > 0){
            /* La primera vez el padre creara el bloque de memoria compartida */
            if(i == 0){
                /*  Obtenemos la clave para poder crear la zona compartida */
                key = ftok(FILEKEY, KEY);
                if(key == -1){
                    printf("Error al obtener key\n");
                    exit(EXIT_FAILURE);
                }
                
                /* Creacion de la zona compartida */
                id_zone = shmget(key, sizeof(Info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
                if(id_zone == -1){
                    id_zone = shmget(key, sizeof(Info), IPC_CREAT | SHM_R | SHM_W);
                    if(id_zone == -1){
                        printf("Error al crear la zona de memoria compartida\n");
                        exit(EXIT_FAILURE);
                    }
                }
                
                /* Unimos la memoria */
                datos = shmat(id_zone, (char*)0, SHM_R | SHM_W);
                if(datos == NULL){
                    printf("Error al unir la memoria\n");
                    exit(EXIT_FAILURE);
                }
            }
            
            if((pid = fork()) == -1){
                printf("Error en el fork\n");
                exit(EXIT_FAILURE);
            }
            
            /* El padre realiza el proceso de la señal SIG_USR1 pedido */
            if(pid > 0){
                if(signal(SIGUSR1, capturar) == SIG_ERR){
                    printf("Error al capturar la señal SIGUSR1");
                    exit(EXIT_FAILURE);
                }
                pause();
            }
        } 
        
        /* En este if solo entraran los hijos y realizan lo pedido en el enunciado*/
        if(pid == 0){
            sleep(2);
            datos = shmat(id_zone, (char*)0, SHM_R | SHM_W);
            if(datos == NULL){
                printf("Error al unir la memoria\n");
                exit(EXIT_FAILURE);
            }
            
            printf("Introduzca el nombre de usuario:\n");
            scanf("%s", (*datos).nombre);
            
            if(i == 0){
                (*datos).id = getpid();
            } else{
                (*datos).id++;
            }
            
            shmdt((char*) datos);
            kill(getppid(), SIGUSR1);
            exit(EXIT_SUCCESS);
        }
    }

    /*
    * Finalizacion del trabajo (eliminacion de todo lo usado)
    */
    shmdt((char*) datos);
    shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
    exit(EXIT_SUCCESS);
}
