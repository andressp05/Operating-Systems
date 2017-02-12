#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 3

int main (void){
   int fpid;
   int ppid;
   int pid;
   int i;
   for (i=0; i < NUM_PROC; i++){
      if ((fpid = fork()) < 0 ){
         printf("Error al emplear fork\n");
         exit(EXIT_FAILURE);
      }
       else if (fpid == 0){
         pid = getpid();
         ppid = getppid();
         printf("PADRE %d del HIJO %d\n", ppid, pid);
       }else{
         pid = getpid();
         printf("PADRE %d\n", pid);
       }getchar();
   }
wait(NULL);
exit(EXIT_SUCCESS);

}