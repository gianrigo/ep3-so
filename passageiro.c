#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "sm.h"
#include "semaph.h"

#define ME					1 /* Semáforo Margem esquerda */
#define MD					2 /* Semáforo Margem direita */
#define EMBARCA			3 /* Semáforo para o barco proceder com o embarque */
#define ATRAVESSA		4 /* Semáforo para o barco proceder com a travessia */
#define DESEMBARCA	5 /* Semáforo para o barco proceder com o desembarque */
#define MUTEX				6	/* Semáforo para exclusão mútua */
#define ESQUERDA 0
#define DIREITA 1
#define TRAVESSIA 2

int sem_id, pid, partida;

/* Aqui o passageiro embarca na margem especificada se possível, ou espera o barco chegar à sua margem do rio */
void embarca(int margem){

	if(margem == ESQUERDA){
		printf("Passageiro %d aguardando na margem ESQUERDA\n",pid);
		waitForAllSPH(sem_id,ME);
	}
	else{
		printf("Passageiro %d aguardando na margem DIREITA\n",pid);
		waitForAllSPH(sem_id,MD);
	}

	/* Se for possível entrar no barco, o passageiro embarca */
	printf("Passageiro %d aguardando para embarcar\n",pid);
	waitSPH(sem_id,EMBARCA);
	printf("Passageiro %d embarcou.\n",pid);                  
	/* Fica aguardando a lotação completa para fazer a travessia */
	decValSPH(sem_id,ATRAVESSA);
	printf("Passageiro %d aguardando a travessia.\n",pid);  
	waitForAllSPH(sem_id,ATRAVESSA);        
}
/* Aqui o passageiro desembarca do barco vindo da margem especificada e realiza quaisquer outras tarefas para dar continuidade à viagem de outros passageiros */
void desembarca(int margem)
{
	decValSPH(sem_id,DESEMBARCA);
	printf("Passageiro %d aguardando o desembarque.\n",pid);  
	waitForAllSPH(sem_id,DESEMBARCA); 
	printf("Passageiro %d desembarcou.\n",pid);  

	/* Reinicia os semáforos */
	
	waitSPH(sem_id,MUTEX);
	if( getValSPH(sem_id,(margem+1)) == 0){ 
		if( margem == ESQUERDA){
			decValSPH(sem_id,MD);
			incValSPH(sem_id,ME);
		}
		else{
			decValSPH(sem_id,ME);
			incValSPH(sem_id,MD);
		}

	setValSPH(sem_id, EMBARCA, 3);
	setValSPH(sem_id, ATRAVESSA, 3);
	setValSPH(sem_id, DESEMBARCA, 3);
	}
	incValSPH(sem_id,MUTEX);
}

/* O barco atravessa o rio a partir da margem especificada */
void atravessa(int margem){

	if( margem == ESQUERDA)
		printf("O passageiro %d está atravessando da margem esquerda para a margem direita do rio.\n",pid);
	else
		printf("O passageiro %d está atravessando da margem direita para a margem esquerda do rio.\n",pid);
}

int main(int argc, char *argv[]){
/*	int i;
  int inicializado = 0;
  key_t key;
  struct semid_ds semds;
  union semun semopts;  */

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

  /*if ((key = ftok("passageiro.c", 'R')) == -1) {
    printf("Não foi possível obter a chave.\n");
    exit(-1);
  }

  if((sem_id = semget(key, 3, 0666 | IPC_CREAT | IPC_EXCL) == EEXIST){
    semopts.buf = &semds;

   */ /*Se chegou aqui, significa que conjunto de semáforos já foi criado.*/
   /* while(!inicializado){*/
      /*Espera a inicialização dos semáforos*/
      /*inicializado = 1;

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
  }else{*/
    /*por padrão, vamos inicializar o barco na margem esquerda*/
    /*semopts.val = 3;
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
  }*/
	if ( (sem_id = createSPH(6)) == -1)
		sem_id = getIdSPH();
	/* Caso os semáforos não existiam, estes foram criados na chamada da função createSPH e serão inicializados abaixo */
  else{
		setValSPH(sem_id, ME, 0); /* Barco inicia na margem esquerda */
		setValSPH(sem_id, MD, 1);
		setValSPH(sem_id, EMBARCA, 3);
		setValSPH(sem_id, ATRAVESSA, 3);
		setValSPH(sem_id, DESEMBARCA, 3);
		setValSPH(sem_id, MUTEX, 1);
	}

	embarca(partida);
	atravessa(partida);
	desembarca(partida);
	/* imprime passageiro saiu do pier */
	exit(0);
}

