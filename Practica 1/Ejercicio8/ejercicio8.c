#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main(int argc, char* argv[]){
	int i;
	int fpid = 1;
    int pid;
    int ppid;
    char *aux[2];

	for(i = 0; i < argc; i++){
		if (fpid == 0){
            if(strcmp("-l", argv[argc-1]) == 0){
            	execl(argv[i],argv[i], NULL);
            }
            else if(strcmp("-lp", argv[argc-1]) == 0){
            	execlp(argv[i], argv[i], NULL);
            }
            else if(strcmp("-v", argv[argc-1]) == 0){
            	aux[0]=argv[i];
            	aux[1]=NULL;
            	execv(argv[i],aux);
            }
            else if(strcmp("-vp", argv[argc-1]) == 0){
            	aux[0]=argv[i];
            	aux[1]=NULL;
            	execvp(argv[i], aux);
            }
        }else{
            if ((fpid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }
            
        }
	}
}