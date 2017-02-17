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
        printf("Introduce una cadena\n");
        fgets(cad, CADENA, stdin);
        printf("\n%s\n", cad);
        free(cad);
    }
    else{
        wait(NULL);
        free(cad);
    }
} 


/* El proceso padre no tiene acceso a ese valor ya que el fork
duplica la informacion del padre en el hijo y despues ambos procesos
trabajan en paralelo*/

/*La memoria se libera tanto en el padre tras crear a su hijo como en
el hijo una vez que ha terminado su proceso.*/