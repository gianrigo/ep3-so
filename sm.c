#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "sm.h"

typedef struct {
	int passageiros;
} sharedData;

static struct {
	int id;
	sharedData *sd;
} data = { -1, NULL };


key_t generateKey(){
	return ftok("passageiro.c", 'R');
}

int getIdSM(){
	return shmget(generateKey(), SHM_SIZE, 0644 | IPC_CREAT);
}

void* attachSM(){
	return shmat(getIdSM(), (void *)0, 0);
}

int getPassengersSM(){
	return data.sd->passageiros;
}
void setPassengersSM(){
	data.sd->passageiros++;
}

void decPassengersSM(){
	data.sd->passageiros--;
}

int creatorSM(){
	int shmid;

	shmid = getIdSM();
	if (shmid == -1) {
		printf("Shmget falhou.\n");
		return -1;
	}
	data.sd = (sharedData*) shmat(data.id, 0, 0);
	data.id = shmid;
	data.sd->passageiros = -1;
	return 0;
}
void removeSM(){
	if (shmdt((void *)0) == -1) {
		printf("Shmdt falhou.\n");
		exit(EXIT_FAILURE);
	}
}
