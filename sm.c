#include <stdio.h>
#include <stdlib.h>
#include "sm.h"
struct sharedData {
	int id;
	int passageiros;
} data = {-1,-1};

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
	return data.passageiros;
}
void setPassengersSM(){
	data.passageiros++;
}

void decPassengersSM(){
	data.passageiros--;
}

int creatorSM(){
	/*void *sm = (void *)0;*/
	int shmid;

	shmid = getIdSM();
	if (shmid == -1) {
		printf("Shmget falhou.\n");
		return -1;
	}

	data.id = shmid;
	data.passageiros = 0;
	return 0;
}
void removeSM(){
	if (shmdt((void *)0) == -1) {
		printf("Shmdt falhou.\n");
		exit(EXIT_FAILURE);
	}
}
