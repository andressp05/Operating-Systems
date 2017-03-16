/**
 * @brief Implementa el ejercicio 8 de comunicacion de procesos con variables globales
 * @file ejercicio8b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 16/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

/*Declaracion Variables Globales*/

/**
* @brief Definicion de la variable global pid
*/
int pid = 0; /*pid del proceso*/
/**
* @brief Definicion de la variable global hpid
*/
int hpid = 0; /*pid del hijo del proceso*/
/**
* @brief Definicion de la variable global pid_raiz
*/
int pid_raiz = 0; /*pid de la raiz de los procesos*/
/**
* @brief Definicion de la variable global cont
*/
int cont = 0; /*contador*/
/**
* @brief Definicion de la variable global V
*/
int V; /*parametro pasado por el usuario*/

/**
* @brief funcion del manejador de SIGTERM
* @param sig señal pasada
*/
void manejador_SIGTERM(int sig){
    sleep(1);
    if(getpid() == pid_raiz){
        return;
    }
    if(hpid > 0){
        kill(hpid, SIGTERM);
    } else {
        kill(pid_raiz, SIGTERM);
    }
    printf("Muere %d\n", pid);
    fflush(stdout);
    wait(NULL);
    exit(EXIT_SUCCESS);
}

/**
* @brief funcion del manejador de SIGUSR1
* @param sig señal pasada
*/
void manejador_SIGUSR1(int sig){
    time_t tiempo;
    struct tm *tlocal;
    char output[128];
    
    if(getpid() == pid_raiz){
        if(cont == V){
            return;
        }
        cont++;
    }
    
    tiempo = time(0);
    tlocal = localtime(&tiempo);
    strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);
    printf("Hola PID=%d,time=%s\n", getpid(), output);
    fflush(stdout);
    sleep(2);
    if(hpid > 0){
        kill(hpid, SIGUSR1);
    } else{
        kill(pid_raiz, SIGUSR1);
    }
    return;
}

/**
* @brief funcion de comunicacion de procesos
* @param argc contiene el número de parámetros totales pasados
* @param argv[] contiene los parámetros pasados por el usuario
* @return int: valor de exito o fracaso
*/
int main (int argc, char *argv[]){
    int fpid = 0;
    int i, j;
    int N;
    
    /*Comprobacion Errores*/
    if(argc != 3){
        printf("Se debe pasar dos parámetros\n");
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    if(N <= 0){
        printf("El primer parámetro ha de ser un entero positivo.\n");
        exit(EXIT_FAILURE);
    }
    
    V = atoi(argv[2]);
    if(V <= 0){
        printf("El segundo parámetro ha de ser un entero positivo.\n");
        exit(EXIT_FAILURE);
    }
    
    pid_raiz = getpid();
    
    /*Creacion de los N procesos*/
    for(i = 0; i < N; i++){
        if(fpid == 0){
            if ((fpid = fork()) == -1){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            
            if(fpid > 0){
                pid = getpid();
                hpid = fpid;
                if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){
                    printf("Error en la señal SIGUSR1\n");
                    exit(EXIT_FAILURE);
                }
                pause();
            } else{
                pid = getpid();
            }
        }
    }
    
    /*Caso ultimo proceso*/
    if(hpid == 0){
        sleep(5);
        kill(pid_raiz, SIGUSR1);
        if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
        pause();
    }
    
    /*Resto procesos V veces los N procesos*/
    for(i = 0; i < V; i++){
        if(signal(SIGTERM, manejador_SIGTERM) == SIG_ERR){
            printf("Error en la señal SIGTERM\n");
            exit(EXIT_FAILURE);
        }
        if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
        pause();
    }
    
    /*protocolo de terminacion*/
    if(getpid() == pid_raiz){
        sleep(1);
        kill(hpid, SIGTERM);
        if(signal(SIGTERM, manejador_SIGTERM) == SIG_ERR){
            printf("Error en la señal SIGTERM\n");
            exit(EXIT_FAILURE);
        }
        pause();
        printf("Muere %d\n", pid);
        fflush(stdout);
        wait(NULL);
    }
    
    exit(EXIT_SUCCESS);
}