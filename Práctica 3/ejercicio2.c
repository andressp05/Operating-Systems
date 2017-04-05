#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>

#define KEY 1300
#define FILEKEY "/bin/cat"

typedef struct info{
	char nombre[80];
	int id;
}Info;

Info* datos = NULL;

void capturar(int signal){
    printf("Nombre de usuario: %s\n", (*datos).nombre);
    printf("Identificador: %d\n", (*datos).id);
}

int main(int argc, char* argv[]){
    int i;
    int n;
    int pid = 1;
    int id_zone;
    key_t key;
    
    if(argc != 2){
        printf("Se debe pasar un número entero positivo como único argumento\n");
        exit(EXIT_FAILURE);
    }
    
    if((n = atoi(argv[1])) <= 0){
        printf("El argumento de entrada debe ser un entero positivo\n");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < n; i++){
        if(pid > 0){
            if(i == 0){
                key = ftok(FILEKEY, KEY);
                if(key == -1){
                    printf("Error al obtener key\n");
                    exit(EXIT_FAILURE);
                }
                
                id_zone = shmget(key, sizeof(Info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
                if(id_zone == -1){
                    id_zone = shmget(key, sizeof(Info), IPC_CREAT | SHM_R | SHM_W);
                    if(id_zone == -1){
                        printf("Error al crear la zona de memoria compartida\n");
                        exit(EXIT_FAILURE);
                    }
                }
                
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
            
            if(pid > 0){
                if(signal(SIGUSR1, capturar) == SIG_ERR){
                    printf("Error al capturar la señal SIGUSR1");
                    exit(EXIT_FAILURE);
                }
                pause();
            }
        } 
        
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
    shmdt((char*) datos);
    shmctl(id_zone, IPC_RMID, (struct shmid_ds*) NULL);
    exit(EXIT_SUCCESS);
}
