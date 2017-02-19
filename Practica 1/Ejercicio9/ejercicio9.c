#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define NUM_PROC 4
int main (void){
    int fpid = 1;
    int pid;
    int ppid;
    int i;
    for (i=0; i < NUM_PROC; i++){
        if (fpid == 0){
            pid = getpid();
            ppid = getppid();
            printf("HIJO %d: PID PADRE: %d\t PID HIJO: %d\n", i, ppid, pid);
        }else{
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            pid = getpid();
            printf ("PADRE %d: PID %d \n", i, pid);
        }
    }
    wait(NULL);
    getchar();
    exit(EXIT_SUCCESS);
} 
