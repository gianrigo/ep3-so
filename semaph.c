#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sm.h"

union semun {
	int val ;
	struct semid_ds buf[2] ;
	unsigned short int array[4] ;
	struct seminfo *__buf; 
};

/* Cria um conjunto (qtd) de semáforos e retorna o semid, ou retorna -1 se o semáforo já existir */
int createSPH(int qtd){
	int semid;

	if (( semid = semget(generateKey(), qtd,IPC_CREAT|IPC_EXCL|0666)) == -1) {
		if( errno != EEXIST){
			perror("Erro de semget") ;
			exit(-1) ;
		}
     	}
	return semid;
}
/* Devolve o semid do semáforo */
int getIdSPH(){
	int semid;

	if (( semid = semget(generateKey(),0,0)) == -1 ) {
		perror ("Error semget()") ;
		exit(-1) ;
      	}
	return semid;
}

int getValSPH(int semid, int snum){
	int semval;
	union semun arg;

	if ( (semval = semctl(semid,snum-1,GETVAL,arg)) == -1){
		perror("Error semctl() GETVAL") ;
		exit(-1) ;
	}
	return semval;
}

void setValSPH(int semid, int snum, int val){
	struct sembuf sempar;
	sempar.sem_num = snum-1 ;
	sempar.sem_op = val;
	sempar.sem_flg = 0;

	if (semop(semid, &sempar, 1) == -1) {
		perror("Error semop()") ;
		exit(-1);
	}
}

void waitSPH(int semid, int snum){
	struct sembuf sempar;
	sempar.sem_num = snum-1 ;
	sempar.sem_op = -1;
     	sempar.sem_flg = 0;

	if (semop(semid, &sempar, 1) == -1) {
		perror("Error semop()") ;
		exit(-1);
	}
}

void waitForAllSPH(int semid, int snum){
	struct sembuf sempar;
	sempar.sem_num = snum-1 ;
	sempar.sem_op = 0;
     	sempar.sem_flg = 0;

	if (semop(semid, &sempar, 1) == -1) {
		perror("Error semop()") ;
		exit(-1);
	}
}

/* Incrementa o valor do semáforo */
void incValSPH(int semid, int snum){
	setValSPH(semid, snum, 1);
}

/* Decrementa o valor do semáforo */
void decValSPH(int semid, int snum){
	setValSPH(semid, snum, -1);
}

    /*
     * destruicao do semaforo
     */
     /*if (semctl(semid,0,IPC_RMID,0)==-1){
             perror("Impossivel de destruir o semaforo") ;
             exit(1) ;
     }
     else printf("O semaforo com semid %d foi destruido\n",semid) ;*/
	/*if( getValSPH(semid,3) == 0 )*/
