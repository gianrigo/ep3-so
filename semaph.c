#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sm.h"
#define _GNU_SOURCE

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
/*int waitTimedSPH(int semid, int snum, struct timespec *timeout){
	struct sembuf sempar;
	sempar.sem_num = snum-1 ;
	sempar.sem_op = -1;
	sempar.sem_flg = 0;
	printf("TIME %d \n",timeout->tv_sec);
	if (semtimedop(semid, &sempar, 1,timeout) == -1) {
		if( errno == EAGAIN)
			return -1;
		perror("Error semop()") ;
		exit(-1);
	}
	printf("TIME %d \n",timeout->tv_sec);
	return 0;
}
*/
int waitForAllTimedSPH(int semid, int snum){
	struct sembuf sempar;
	struct timespec timeout;
	sempar.sem_num = snum-1 ;
	sempar.sem_op = 0;
	sempar.sem_flg = 0;

	timeout.tv_sec = 10;

	if (semtimedop(semid, &sempar, 1,&timeout) == -1) {
		if( errno == EAGAIN)
			return -1;
		perror("Error semop()") ;
		exit(-1);
	}
	return 0;
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
void removeSPH(int semid){
	if (semctl(semid,0,IPC_RMID,0) == -1){
		perror("Impossível remover o semáforo");
		exit(-1);
	}
	else
		printf("O semáforo com semid %d foi destruído.\n",semid);
}
