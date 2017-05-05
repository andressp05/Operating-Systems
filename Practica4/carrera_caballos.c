#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <msg.h>

#define LEER 0
#define ESCRIBIR 1;


#define ERROR -1

/**
* @brief Definicion de la clave
*/
#define KEY 1300

/**
* @brief Definicion de la clave de fichero
*/
#define FILEKEY "/bin/cat"

/**
* @brief Definicion de la clabe del semaforo
*/
#define SEMKEY 75798

/**
* @brief Numero de semaforos a crear
*/
#define N_SEMAFOROS 1


typedef struct _Apuesta{
	long id; /*!<Tipo de mensaje*/
	/*Informacion a transmitir en el mensaje*/
	char[20] nombre;
	int numCaballo;
	double cuantia;
} apuesta;

typedef struct _Caballo{
	int id; /*!<Identificador del caballo*/
	double totalapostado; /*!<Total Dinero apostado al caballo*/
	double cotizacion; /*!<Cotizacion del caballo*/
} caballo;


void ventanilla(void* arg); /* Hilo de las ventanillas */

void pantalla(void* args); /* Hilo del monitor */

void gestor(int numApostadores); /* Funcion que ejecuta el proceso hijo gestor de apuestas. Crea la cola de mensajes y un proceso hijo apostador ejecuta la funcion apostador */

void apostador(int msgid, int numApostadores, int numCaballos); /* Funcion que ejecuta el proceso apostador. Recibe el id de la cola de mensajes */

/* Funcion ejecutada por cada hijo caballo. Devuelve la tirada */
int caballo(int pos, int maxpos){
    if(pos > maxpos || pos <= 0){
        return ERROR;
    }

    if(pos == 1){
        return rand()%7+1;
    }

    if(pos == maxpos){
        return rand()%12+1;
    }

    return rand()%6+1;
}

void main(int argc, char* argv[]){
    int i;
    int numCaballos;
    int longitud;
    int apostadores;
    int ventanillas;
    int pid;
    int* caballos;
    int estado;
    int* cpids;
    int padre[2];
    int padre_status;
    int hijo[2];
    int hijo_status;

    if(argc != 4){
        printf("Se deben introducir cuatro parámetros enteros positivos:\nNúmero de caballos participantes (máximo 10)\n");
        printf("Longitud de la carrera\n, Número de apostadores (máximo 10)\n, Número de ventanillas\n");
        exit(EXIT_FAILURE);
    }

    if((numCaballos = atoi(argv[1]) <= 0 || numCaballos > 10){
        printf("El primer parámetro indica el número de caballos. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((longitud = atoi(argv[2]) <= 0){
        printf("El segundo parámetro indica la longitud de la carrera. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }

    if((apostadores = atoi(argv[3]) <= 0 || apostadores > 10){
        printf("El tercer parámetro indica el número de apostadores. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((ventanillas = atoi(argv[4]) <= 0){
        printf("El cuarto parámetro indica el número de ventanillas. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }

    padre_status = pipe(fd);
    if(padre_status == ERROR){
    	perror("Error creando la tubería\n");
    	exit(EXIT_FAILURE);
    }

    hijo_status = pipe(fd);
    if(hijo_status == ERROR){
    	perror("Error creando la tubería\n");
    	exit(EXIT_FAILURE);
    }

    cpids = (int*)malloc(sizeof(int)*numCaballos);
    if(!cpids){
    	printf("Error al reservar memoria");
    	exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) == ERROR){
        printf("Error al crear el monitor\n");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0){
        /*MONITOR*/
    }

    if((pid = fork()) == ERROR){
        printf("Error al crear el gestor de apuestas\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        /*GESTOR*/

    }
    
    for(i = 0; i < numCaballos; i++){
        if((cpids[i] = fork()) == ERROR){
            printf("Error al crear el gestor de \n");
            exit(EXIT_FAILURE);
        }
    }

    if(cpid[i] == 0){
    	char[100] pos;
    	char[100] tiro;
    	pause();
    	close(padre[ESCRIBIR]);
    	read(padre[LEER], pos, sizeof(pos));
    	int tirada = caballo(atoi(pos), numCaballos);
    	close(hijo[LEER]);
    	sprintf(tiro, "%d", tirada);
    	write(hijo[ESCRIBIR], tiro, strlen(tiro));
    } else {
    	for(i = 0; i < numCaballos; i++){
    		kill(SIGUSR1, cpids[i]);

    	}
    }

    sleep(15);
}

void ventanilla(void* arg); /* Hilo de las ventanillas */

void pantalla(void* args); /* Hilo del monitor */

void gestor(int numApostadores, int numCaballos){
	int msqid;
	int ret;
    key_t key;
    pthread_t h;
    int i,j;
    int sem_id; /* ID de la lista de semáforos */
    struct sembuf sem_oper; /* Para operaciones up y down sobre semáforos */
    union semun {
		int val;
		struct semid_ds *semstat;
		unsigned short *array;
	} arg;
		
    caballo** caballos;
    apuesta amsg;
    apuesta[150] aps;

    caballos = (caballo**)malloc(numCaballos*sizeof(caballo*));
    if(!caballos){
    	perror("Error al reservar memoria");
    	exit(EXIT_FAILURE);
    }

    /*  Obtenemos la clave para poder crear la cola de mensajes */ 
	key = ftok(FILEKEY, KEY);
    if(key == -1){
    	perror("Error al obtener key\n");
    	exit(EXIT_FAILURE);
    }

    /* Creacion de la cola de mensajes */
    msqid = msgget(key, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if(msqid == -1){
    	msqid = msgget(key, IPC_CREAT | SHM_R | SHM_W);
        if(msqid == -1){
        	perror("Error al crear la cola de mensajes\n");
           	exit(EXIT_FAILURE);
       	}
    }

    /* Creacion de semaforos */
    semid = semget(SEMKEY, N_SEMAFOROS,IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	if((semid == -1) && errno == EEXIST)
	semid = semget(SEMKEY,N_SEMAFOROS,SHM_R|SHM_W);
	if(semid==-1){
		perror("semget");
		exit(errno);
	}

	/* Inicializamos los semáforos */
	arg.array = (unsigned short *)malloc(sizeof(short)*N_SEMAFOROS);
	arg.array [0] = 1;
	semctl (semid, N_SEMAFOROS, SETALL, arg);

    /* Inicializamos las apuestas */
    for (i=0; i < numCaballos; i++){
    	caballos[i] = (caballo*)malloc(sizeof(caballo));
    	if(!caballos[i]){
    		
    		for(j=0;j<i;j++){
    			free(caballos[j]);
    		}
    		free(caballos);

    		perror("Error al reservar memoria");
    		exit(EXIT_FAILURE);		
    	}

    	caballos[i].id = i+1;
    	caballos[i].totalapostado = 1.0;
    	caballos[i].cotizacion = numCaballos;
    }


    /*/* Falta dinero a pagar a cada apostador para cada caballo */


    /*Inicializa los threads de las ventanillas*/
    for(i=0; i<numVentanillas; i++){
    	ret = pthread_create(&h, NULL, ventanilla, (void*) aps);
    	
    	if(ret){
        	printf("Error al crear el hilo %d\n", i+1);
        	exit(EXIT_FAILURE);
    	}
    
    	pthread_join(h,NULL);
    	pthread_cancel(h);
	}

	if((pid = fork()) == ERROR){
        printf("Error al crear el apostador\n");
        exit(EXIT_FAILURE);
    }

    if(pid > 0){ /* Gestor*/ 
    	msgrcv (msqid, (struct msgbuf *) &amsg, sizeof(apuesta) - sizeof(long), 1, 0);
    	
    }

    else{
    	apostador(msqid,numApostadores,numCaballos);
    }
}


void apostador(int msgid, int numApostadores, int numCaballos){
	int aleat;
	aleat = randint() % numApostadores+1;
	apuesta msg;
	msg.
}