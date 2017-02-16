#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define CADENA 80
int main (void){
    int pid;
    char* cad;
    
    cad = (char*)malloc(CADENA*sizeof(char));
    if ((pid=fork()) <0 ){
        printf("Error al emplear fork\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0){
        fgets(cad, CADENA, stdin);
        printf("\n%s\n", cad);
        free(cad);
    }
    else{
        wait(NULL);
        free(cad);
    }
} 