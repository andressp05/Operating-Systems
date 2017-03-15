/**
 * @brief Implementa el ejercicio 5b de procesos con un solo padre
 * @file ejercicio5b.c
 * @author Andres Salas andres.salas@estudiante.uam.es
 * @author Antonio Martin antonio.martinmasuda@estudiante.uam.es
 * @note Grupo 2202
 * @version 1.0
 * @date 10/02/2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

void captura(int sig){
    return;
}

/**
* @brief funcion de procesos con un solo padre
* @return int: valor de exito o fracaso
*/
int main (int argc, char *argv[]){
    int fpid = 0;
    int i, j;
    int N;
    int V;
    int* array;
    time_t tiempo;
    struct tm *tlocal;
    char output[128];
    
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

    array = (int*)malloc(sizeof(int)*(N+1));
    if(array == NULL){
        printf("Error reservando memoria.\n");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < N+1; i++){
        array[i] = 0;
    }

    array[0] = getpid();
    for(i = 0; i < N; i++){
        if(fpid == 0){
            if ((fpid = fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            
            if(fpid > 0){
                array[i] = getpid();
                array[i+1] = fpid;
                if(signal(SIGUSR1, captura) == SIG_ERR){
                    printf("Error en la señal SIGUSR1\n");
                    exit(EXIT_FAILURE);
                }
                pause();
            } else{
                array[i+1] = getpid();
            }
        }
    }
    
    if(getpid() == array[N]){
        fflush(stdout);
        kill(array[0], SIGUSR1);
        if(signal(SIGUSR1, captura) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
        pause();
    }

    for(j = 0; j < V; j++){
        for (i = 0; i < N; i++){
            if(getpid() == array[i]){
                tiempo = time(0);
                tlocal = localtime(&tiempo);
                strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);
                printf("Hola PID=%d,time=%s\n", getpid(), output);
                fflush(stdout);
                sleep(2);
                kill(array[i+1],SIGUSR1);
                break;
            }
        }

        if(getpid() == array[N]){
            tiempo = time(0);
            tlocal = localtime(&tiempo);
            strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);
            printf("Hola PID=%d,time=%s\n", getpid(), output);
            fflush(stdout);
            sleep(5);
            kill(array[0],SIGUSR1);
        }
        if(signal(SIGUSR1, captura) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
    
        if(signal(SIGTERM, captura) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
        pause();
    }
    
    
    if(getpid() == array[0]){
        kill(array[1],SIGTERM);
        if(signal(SIGTERM, captura) == SIG_ERR){
            printf("Error en la señal SIGUSR1\n");
            exit(EXIT_FAILURE);
        }
        pause();
        printf("Muere %d\n", array[0]);
        free(array);
        exit(EXIT_SUCCESS);
    }
    
    for(i = 1; i < N; i++){
        if(getpid() == array[i]){
            sleep(1);
            kill(array[(i+1)],SIGTERM);
            printf("Muere %d\n", array[i]);
            free(array);
            exit(EXIT_SUCCESS);
        }
    }
    
    if(getpid() == array[N]){
        sleep(1);
        kill(array[0],SIGTERM);
        printf("Muere %d\n", array[N]);
        free(array);
        exit(EXIT_SUCCESS);
    }
    
    exit(EXIT_SUCCESS);
}