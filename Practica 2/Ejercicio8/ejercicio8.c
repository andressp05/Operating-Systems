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

int time() {
time_t tiempo = time(0);
struct tm *tlocal = localtime(&tiempo);
char output[128];
strftime(output, 128, "%d/m/%y %H:%M:%S", tlocal);
printf("Fecha y Hora: %s",output);
return 0;
}
/**
* @brief funcion de procesos con un solo padre
* @return int: valor de exito o fracaso
*/
int main (int argc, char *argv[]){
    int fpid = 1;
    int i;
    int ppid;
    int N;
    int* array;
    
    if(argc != 3){
        printf("Se debe pasar un solo parámetro\n");
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);

    array = (int*)malloc(sizeof(int)*N);

    array[0] = getpid();
    for(i=0; i < atoi(argv[1]); i++){
        
        if(fpid == 0){
            if ((fpid = fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            if(fpid != 0){
                array[i+1] = fpid;    
            }
        }
    }

    for(int j = 0; j <atoi(argv[2])){
        for (int i = 0; i < N-2; i++){
            if(getpid() == array[i]){
                printf("Hola PID=%d,time=%s\n", getpid(), );
                sleep(2);
                kill(getppid(),SIGUSR1);
            }
        }

        if(getpid() == array[i]){
            sleep(5);
            kill(array[0],SIGUSR1);
        }
    }
    
    for(i = 0; i < N-1; i++){
        if(getpid() == array[i]){
            wait()
    }
    wait(NULL);
    getchar();
    exit(EXIT_SUCCESS);
}