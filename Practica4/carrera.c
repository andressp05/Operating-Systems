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
#include <sys/msg.h>
#include <errno.h>

#define LEER 0
#define ESCRIBIR 1

#define ERROR -1

#define EMPEZADA 0
#define FINALIZADA 1
#define MAX 10

/**
* @brief Definicion de la clave
*/
#define KEY 12

#define KEYSHM 14

/**
* @brief Definicion de la clave de fichero
*/
#define FILEKEY "/bin/cat"

/**
* @brief Definicion de la clabe del semaforo
*/
#define SEMKEY 75798

typedef struct _Apuesta{
	long id; /*!<Tipo de mensaje*/
	/*Informacion a transmitir en el mensaje*/
	char nombre[20];
	int numCaballo;
	double cuantia;
} apuesta;

typedef struct _Caballo{
	int id; /*!<Identificador del caballo*/
	double totalapostado; /*!<Total Dinero apostado al caballo*/
	double cotizacion; /*!<Cotizacion del caballo*/
} caballo;

typedef struct _Carrera{
    int id; /*!<id del caballo de 1 hasta el numero de caballos*/
    int pid; /*!<pid del proceso correspondiente a este caballo*/
    int pos; /*!<posicion de este caballo*/
    int tirada; /*!<tiradas de este caballo*/
} carrera;

typedef struct _Ventanilla{
    int numCaballos; /*!<numero de caballos*/
    caballo jinetes[MAX]; /*!<array de punteros a la estructura de caballo*/
    double apostadores[MAX][MAX]; /*!<matriz de apostadores-caballo*/
    double total; /*!<total apostado a todos los caballos*/
    int msgid; /*!<id de la cola de mensajes*/
    int semid; /*!<id del array de semaforos*/
    int estado; /*!<estado de la carrera*/
} args;

typedef struct _Monitor{
    int* j;
    int* k;
    int shmid;
    int numCaballos;
    int numApostadores;
    carrera* caballos;
} monitor;

int estado = -1;

int dado(int pos, int maxpos);

void captura_SIGUSR1(int sig);

void captura_SIGUSR2(int sig);

void captura_SIGINT(int sig);

void captura_SIGQUIT(int sig);

void* ventanilla(void* arg); /* Hilo de las ventanillas */

void* pantalla(void* arg); /* Hilo monitor */

void gestor(int shmid, int numCaballos, int numApostadores, int ventanillas, carrera caballos[MAX]); /* Funcion que ejecuta el proceso hijo gestor de apuestas. Crea la cola de mensajes y un proceso hijo apostador ejecuta la funcion apostador */

void apostador(int msqid, int numCaballos, int numApostadores); /* Funcion que ejecuta el proceso apostador. Recibe el id de la cola de mensajes */

void main(int argc, char* argv[]){
    int ret;
    pthread_t pant;
    int shmid;
    int i, l;
    int j = 0;
    int k = -1;
    int numCaballos;
    int longitud;
    int numApostadores;
    int ventanillas;
    int pid;
    int padre_status;
    int hijo_status;
    int padre[2];
    int hijo[2];
    carrera* caballos;
    monitor screen;

    if(argc != 5){
        printf("Se deben introducir cuatro parámetros enteros positivos:\nNúmero de caballos participantes (máximo 10)\n");
        printf("Longitud de la carrera\n, Número de apostadores (máximo 10)\n, Número de ventanillas\n");
        exit(EXIT_FAILURE);
    }

    if((numCaballos = atoi(argv[1])) <= 0 || numCaballos > 10){
        printf("El primer parámetro indica el número de caballos. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((longitud = atoi(argv[2])) <= 0){
        printf("El segundo parámetro indica la longitud de la carrera. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }

    if((numApostadores = atoi(argv[3])) <= 0 || numApostadores > 10){
        printf("El tercer parámetro indica el número de apostadores. Debe ser un número entero positivo menor o igual a diez\n");
        exit(EXIT_FAILURE);
    }

    if((ventanillas = atoi(argv[4])) <= 0){
        printf("El cuarto parámetro indica el número de ventanillas. Debe ser un número entero positivo\n");
        exit(EXIT_FAILURE);
    }
    
    caballos = (carrera*) malloc(sizeof(carrera)*numCaballos);
    if(caballos == NULL){
        printf("Error al reservar memoria para los caballos");
        exit(EXIT_FAILURE);
    }

    padre_status = pipe(padre);
    if(padre_status == ERROR){
    	perror("Error creando la tubería\n");
    	exit(EXIT_FAILURE);
    }

    hijo_status = pipe(hijo);
    if(hijo_status == ERROR){
    	perror("Error creando la tubería\n");
    	exit(EXIT_FAILURE);
    }

    int key = ftok(FILEKEY, KEYSHM);
    if(key == ERROR){
        fprintf(stderr, "Error con la clave");
        exit(EXIT_FAILURE);
    }
    
    shmid = shmget(key, sizeof(args)*1, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if(shmid == ERROR){
        if((shmid = shmget(key, sizeof(args), SHM_R | SHM_W)) == ERROR){
            fprintf(stderr, "Error al crear la zona de memoria compartida\n");
            exit(EXIT_FAILURE);
        }
    }

    if((pid = fork()) == ERROR) {
        printf("Error al crear el gestor de apuestas\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < numCaballos; i++){
        caballos[i].id = i+1;
    }

    if(pid == 0){
        gestor(shmid, numCaballos, numApostadores, ventanillas, caballos);
    }
    screen.j = &j;
    screen.k = &k;
    screen.shmid = shmid;
    screen.numCaballos = numCaballos;
    screen.numApostadores = numApostadores;
    screen.caballos = caballos;
    ret = pthread_create(&pant, NULL, pantalla, (void*) &screen);
    if(ret){
    	printf("Error al crear el hilo %d\n", i+1);
    	exit(EXIT_FAILURE);
    }
    sleep(10);
    printf("LLEGA1");
    kill(pid, SIGQUIT);
    wait(NULL);
    estado = EMPEZADA;
    printf("LLEGA2");

    if(signal(SIGUSR1, captura_SIGUSR1) == SIG_ERR){
        printf("Error en la señal SIGUSR1\n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGUSR2, captura_SIGUSR2) == SIG_ERR){
        printf("Error al capturar la señal SIGUSR2\n");
        exit(EXIT_FAILURE);
    }
    for(l = 0; l < numCaballos; l++){
        if((caballos[l].pid = fork()) == ERROR){
            printf("Error al crear el caballo %d\n", j+1);
            exit(EXIT_FAILURE);
        }
        caballos[l].pos = 0;
        caballos[l].tirada = 0;
        if(caballos[l].pid == 0){
            while(1){
                pause();
                srand(getpid());
                char pos[3];
                char tiro[3];
                int tirada;
                memset(pos, 0, sizeof(pos));
                memset(tiro, 0, sizeof(tiro));
                close(padre[ESCRIBIR]);
                read(padre[LEER], pos, sizeof(pos));
                tirada = dado(atoi(pos), numCaballos);
                fflush(stdout);
                close(hijo[LEER]);
                sprintf(tiro, "%d", tirada);
                write(hijo[ESCRIBIR], tiro, strlen(tiro));
                kill(getppid(), SIGUSR2);
            }
            exit(EXIT_SUCCESS);
        }
    }
    while(estado == EMPEZADA){
        for(i = 0, j = 0; i < numCaballos; i++){
            char pos[3];
            char tiro[3];
            int tirada;
            memset(pos, 0, sizeof(pos));
            close(padre[LEER]);
            sprintf(pos, "%d", caballos[i].pos);
            write(padre[ESCRIBIR], pos, strlen(pos));
    		kill(caballos[i].pid, SIGUSR1);
    		pause();
    		memset(tiro, 0, sizeof(tiro));
            close(hijo[ESCRIBIR]);
            read(hijo[LEER], tiro, sizeof(tiro));
            tirada = atoi(tiro);
            caballos[i].tirada += tirada;
        }
        caballos[0].pos = 1;
        for(i = 0; i < numCaballos-1; i++){
            for(j = i+1; j > 0; j--){
                if(caballos[j].tirada > caballos[j-1].tirada){
                    carrera aux = caballos[j];
                    caballos[j] = caballos[j-1];
                    caballos[j-1] = aux;
                    caballos[j].pos++;
                    caballos[j-1].pos = caballos[j].pos-1;
                } else if(caballos[j].tirada == caballos[j-1].tirada){
                    caballos[j].pos = caballos[j-1].pos;
                } else {
                    caballos[j].pos = caballos[j-1].pos+1;
                }
            }
        }
        k++;
        while(j == k);
        if(caballos[0].tirada >= longitud){
            estado = FINALIZADA;
        }
        if(estado == FINALIZADA){
            for(i = 0; i < numCaballos; i++){
                kill(caballos[i].pid, SIGKILL);
                wait(NULL);
            }
        }
    }
    pthread_join(pant, NULL);
    pthread_cancel(pant);
    exit(EXIT_SUCCESS);
}

void* pantalla(void* arg){
    int i, j, cont;
    args* aps;
    monitor* scr = (monitor*) arg;
    aps = shmat(scr->shmid, (char*)0, 0);
    if(aps == NULL){
        printf("Error en el monitor al unirse a la memoria compartida");
        exit(EXIT_FAILURE);
    }
    while(estado != EMPEZADA){
        for(cont = 0; cont < 10; cont++){
            /*printf("Han pasado %d segundos", cont);
            fflush(stdout);*/
            sleep(1);
        }
        estado = EMPEZADA;
    }
    for(i = 0; i < scr->numCaballos; i++){
        printf("Cotizacion del caballo %d: %lf\n", aps->jinetes[i].id, aps->jinetes[i].cotizacion);
        fflush(stdout);
    }
    while(estado == EMPEZADA){
        while(*scr->j != *scr->k);
        sleep(1);
        for(i = 0; i < scr->numCaballos; i++){
            printf("Caballo %d va en la posicion %d con %d distancia recorrida\n", scr->caballos[i].id, scr->caballos[i].pos, scr->caballos[i].tirada);
            fflush(stdout);
        }
        (*scr->j)++;
    }
    printf("CARRERA FINALIZADA:\n");
    for(i = 0; i < 3; i++){
        printf("Posicion %d: caballo %d con %d recorrido\n", i+1, scr->caballos[i].id, scr->caballos[i].tirada);
        fflush(stdout);
    }
    
    for(i = 0; i < scr->numCaballos; i++){
        if(scr->caballos[i].pos == 1){
            for(j = 0; j < scr->numApostadores; j++){
                if(aps->apostadores[i][j] > 0){
                    printf("Apostador-%d ha ganado %lf euros\n", j+1, aps->apostadores[i][j]);
                    fflush(stdout);
                }
            }
        } else {
            break;
        }
    }
}

void* ventanilla(void* datos){
    args* arg = (args*) datos; 
    double total = arg->total;
    int numBet;
    int msgid = arg->msgid;
    int semid = arg->semid;
    apuesta amsg;
    struct sembuf sem_oper_cab, sem_oper_bet, sem_oper_tot;
    while(arg->estado != EMPEZADA){
        msgrcv(msgid, (struct msgbuf *) &amsg, sizeof(apuesta) - sizeof(long), 1, 0);
        if(amsg.numCaballo > arg->numCaballos || amsg.numCaballo < 0)
            continue;
        sem_oper_cab.sem_num = amsg.numCaballo;
        sem_oper_cab.sem_op = -1;
        sem_oper_cab.sem_flg = 0;
        semop(semid, &sem_oper_cab, 1);
        numBet = amsg.nombre[strlen(amsg.nombre)-1] - '0';
        if(numBet == 0)
            numBet = 10;
        sem_oper_bet.sem_num = numBet+arg->numCaballos-1;
        sem_oper_bet.sem_op = -1;
        sem_oper_bet.sem_flg = 0;
        semop(semid, &sem_oper_bet, 1);
        arg->apostadores[amsg.numCaballo-1][numBet-1] = amsg.cuantia*arg->jinetes[amsg.numCaballo-1].cotizacion;
        arg->jinetes[amsg.numCaballo-1].totalapostado += amsg.cuantia; 
        sem_oper_tot.sem_num = 0;
        sem_oper_tot.sem_op = -1;
        sem_oper_tot.sem_flg = 0;
        semop(semid, &sem_oper_tot, 1);
        arg->jinetes[amsg.numCaballo-1].cotizacion = total/arg->jinetes[amsg.numCaballo-1].totalapostado;
        sem_oper_tot.sem_num = 0;
        sem_oper_tot.sem_op = 1;
        sem_oper_tot.sem_flg = 0;
        semop(semid, &sem_oper_tot, 1);
        sem_oper_bet.sem_num = numBet+arg->numCaballos-1;
        sem_oper_bet.sem_op = 1;
        sem_oper_bet.sem_flg = 0;
        semop(semid, &sem_oper_bet, 1);
        sem_oper_cab.sem_num = amsg.numCaballo;
        sem_oper_cab.sem_op = 1;
        sem_oper_cab.sem_flg = 0;
        semop(semid, &sem_oper_cab, 1);
    }
}

void gestor(int shmid, int numCaballos, int numApostadores, int ventanillas, carrera* caballos){
    int pid;
    int numsems;
	int msqid;
	int semid;
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
		
    apuesta amsg;
    args* aps;

    if(signal(SIGQUIT, captura_SIGQUIT)){
        printf("Error en la señal SIGQUIT\n");
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
    	msqid = msgget(key, SHM_R | SHM_W);
        if(msqid == -1){
        	perror("Error al crear la cola de mensajes\n");
           	exit(EXIT_FAILURE);
       	}
    }
    numsems = numCaballos+numApostadores+1;
    /* Creacion de semaforos */
    semid = semget(SEMKEY, numsems,IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	if((semid == -1) && errno == EEXIST){
	    semid = semget(SEMKEY, numsems, SHM_R|SHM_W);
	    if(semid == ERROR){
	    	perror("semget");
	    	exit(errno);
	    }
	}

	/* Inicializamos los semáforos */
	arg.array = (unsigned short *)malloc(sizeof(short)*numsems);
	for(i = 0; i < numsems; i++){
	    arg.array[i] = 1;
	}
	semctl (semid, numsems, SETALL, arg);

    /*Inicializa los threads de las ventanillas*/
    aps = shmat(shmid, (char*)0, 0);
    if(aps == NULL){
        printf("Error en el gestor al unirse a la memoria compartida");
        exit(EXIT_FAILURE);
    }
    for (i=0; i < numCaballos; i++){
    	aps->jinetes[i].id = caballos[i].id;
    	aps->jinetes[i].totalapostado = 1.0;
    	aps->jinetes[i].cotizacion = numCaballos;
    }
    aps->estado = -1;
    aps->total = numCaballos;
    aps->numCaballos = numCaballos;
    aps->msgid = msqid;
    aps->semid = semid;
    for(i = 0; i < numCaballos; i++){
        for(j = 0; j < numApostadores; j++){
            aps->apostadores[i][j] = 0;
        }
    }
    for(i=0; i<ventanillas; i++){
    	ret = pthread_create(&h, NULL, ventanilla, (void*) aps);
    	
    	if(ret){
        	printf("Error al crear el hilo %d\n", i+1);
        	exit(EXIT_FAILURE);
    	}
	}
	
	if((pid = fork()) == ERROR){
        printf("Error al crear el apostador\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0){
        srand(getpid());
    	apostador(msqid, numCaballos, numApostadores);
    }
	while(estado != EMPEZADA);
	aps->estado = EMPEZADA;
    kill(pid, SIGKILL);
	for(i = 0; i<ventanillas; i++){
	    pthread_join(h,NULL);
        pthread_cancel(h);
	}
}

void apostador(int msqid, int numCaballos, int numApostadores){
	int aleat;
	int ret;
	char num[3];
	aleat = rand() % numApostadores+1;
	apuesta msg;
	while(estado != EMPEZADA){
	    msg.id = 1;
    	strcpy(msg.nombre, "APOSTADOR-");
    	sprintf(num, "%d", aleat);
    	strcat(msg.nombre, num);
    	msg.cuantia = rand()%100+(rand()%100)/100.0;
    	msg.numCaballo = rand()%numCaballos+1;
    	ret = msgsnd(msqid, (struct msgbuf*) &msg, sizeof(apuesta)-sizeof(long), 0);
    	if(ret == ERROR){
    	    printf("error msgsnd %d\n", errno);
    	}
    	sleep(1);
	}
}

/* Funcion ejecutada por cada hijo caballo. Devuelve la tirada */
int dado(int pos, int maxpos){

    if(pos > maxpos || pos < 0){
        return ERROR;
    }
    
    if(pos == 1){
        return (rand()/(RAND_MAX+1.0))*7.0+1;
    }

    if(pos == maxpos){
        return (rand()/(RAND_MAX+1.0))*12.0+1;
    }

    return (rand()/(RAND_MAX+1.0))*6.0+1;
}

void captura_SIGUSR1(int sig){
    return;
}

void captura_SIGUSR2(int sig){
    return;
}

void captura_SIGQUIT(int sig){
    estado = EMPEZADA;
}

void captura_SIGINT(int sig){
    estado = FINALIZADA;
    return;
}
