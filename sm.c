#include <stdio.h>
#include <stdlib.h>
#include "sm.h"

key_t generateKey(){
	return ftok("passageiro.c", 'R');
}

int getIdSM(){
	return shmget(generateKey(), SHM_SIZE, 0644 | IPC_CREAT);
}

void* attachSM(){
	return shmat(getIdSM(), (void *)0, 0);
}

int creatorSM(){
	void *sm = (void *)0;
	int shmid;
	struct sharedData *data;

	shmid = getIdSM();
	if (shmid == -1) {
		printf("Shmget falhou.\n");
		return -1;
	}

	sm = attachSM();
	if (sm == (void *)-1) {
		printf("Shmat falhou.\n");
		return -1;
	}
	
	data = (struct sharedData *)sm;
	data -> id = shmid;
	data -> passageiros = 3;
	data -> margem = 0; /* Mudar para aleatorio */
	
	/*if (shmdt(sm) == -1) {
		printf("Shmdt falhou.\n");
		exit(EXIT_FAILURE);
	}*/
	return 0;
}

