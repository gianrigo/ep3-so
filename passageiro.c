#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ESQUERDA 0
#define DIREITA 1
#define TRAVESSIA 2

int sem_id, pid, partida;

void embarca(int margem)
{
  /* Aqui o passageiro embarca na margem especificada se possível,
                                                             
    ou espera o barco chegar à sua margem do rio */
                             
}
void desembarca(int margem)
{
  /* Aqui o passageiro desembarca do barco vindo da margem especificada
    e realiza quaisquer outras tarefas para dar continuidade à viagem
                                                             
    de outros passageiros */
}

/* O barco atravessa o rio a partir da margem especificada */
void atravessa(int margem){
	if( margem == 0)
		printf("O processo %d está atravessando da margem esquerda para a margem direita do rio.\n",pid);
	else
		printf("O processo %d está atravessando da margem direita para a margem esquerda do rio.\n",pid);
}

int main(int argc, char *argv[]){
	int i;
  int inicializado = 0;
  key_t key;
  struct semid_ds semds;
  union semun semopts;  

	/* lê margem de origem como parâmetro */
	if( argc != 2){ 
		printf("Erro ao ler parâmetro de entrada.\n");
		exit(-1);
	}	
	partida = atoi(argv[1]);
	if( partida != 0 && partida != 1 ){
		printf("Margem inválida.\n");
		exit(-1);
	}

  pid = getpid();

  if ((key = ftok("passageiro.c", 'R')) == -1) {
    printf("Não foi possível obter a chave.\n");
    exit(-1);
  }

  if((sem_id = semget(key, 3, 0666 | IPC_CREAT | IPC_EXCL) == EEXIST){
    semopts.buf = &semds;

    /*Se chegou aqui, significa que conjunto de semáforos já foi criado.*/
    while(!inicializado){
      /*Espera a inicialização dos semáforos*/
      inicializado = 1;

      for(i = 0; i < 3; i++){
        if(semctl(sem_id, i, IPC_STAT, semopts) == -1){
          printf("Não pôde ler o semáforo.\n");
          exit(-1);
        }else{
          if(semds.sem_otime == 0){
            inicializado = 0;
          }
        }
      }
    }
  }else{
    /*por padrão, vamos inicializar o barco na margem esquerda*/
    semopts.val = 3;
    if(semctl(sem_id, ESQUERDA, SET_VAL, sem_opts) == -1){
      printf("Falhou ao inicializar o semáforo.\n");
      exit(-1);
    }

    semopts.val = 0;
    if(semctl(sem_id, DIREITA, SET_VAL, sem_opts) == -1){
      printf("Falhou ao inicializar o semáforo.\n");
      exit(-1);
    }

    semopts.val = 0;
    if(semctl(sem_id, TRAVESSIA, SET_VAL, sem_opts) == -1){
      printf("Falhou ao inicializar o semáforo.\n");
      exit(-1);
    }
  }

	embarca(partida);
	atravessa(partida);
	desembarca(partida);
	/* imprime passageiro saiu do pier */
	exit(0);
}

