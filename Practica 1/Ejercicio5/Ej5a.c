#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define NUM_PROC 3
int main (void){
    int fpid = 0;
    int pid;
    int ppid;
    int i;
    for (i=0; i < NUM_PROC; i++){
        if (fpid ==0){
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            pid = getpid();
            ppid = getppid();
            printf("PID PADRE: %d\t PID HIJO: %d\n", ppid, pid);
		}else{
            pid = getpid();
            printf ("PID PADRE %d \n", pid);
        }
    }
    wait(NULL);
    getchar();
    exit(EXIT_SUCCESS);
} 