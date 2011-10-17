#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
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
int desistiu = 0;

void checkOut(){
	if(	(getValSPH(sem_id, EMBARCA) == 3) && (getValSPH(sem_id, ATRAVESSA) == 3) &&	(getValSPH(sem_id, DESEMBARCA) == 3) && (getInfoSPH(sem_id, 1) == 0) && (getInfoSPH(sem_id, 2) == 0) && (getInfoSPH(sem_id, 6) == 0)  ){
		printf("Como não há alguem embarcando, atravessando ou desembarcando, os recursos serão eliminados pelo passageiro %d.\n",pid);
		removeSPH(sem_id);
	}
}
void giveUp(){
	printf("Passageiro %d desistiu.\n",pid);
	desistiu = 1;
	checkOut();
}

/* Aqui o passageiro embarca na margem especificada se possível, ou espera o barco chegar à sua margem do rio */
void embarca(int margem){

	if(margem == ESQUERDA){
		printf("Passageiro %d aguardando na margem ESQUERDA\n",pid);
		if(waitForAllTimedSPH(sem_id,ME) == -1){
			giveUp();
			return;
		}
	}
	else{
		printf("Passageiro %d aguardando na margem DIREITA\n",pid);
		if(waitForAllTimedSPH(sem_id,MD) == -1){
			giveUp();
			return;
		}
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
		printf("Passageiro %d está atualizando os semáforos.\n",pid);
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
	checkOut();
}

/* O barco atravessa o rio a partir da margem especificada */
void atravessa(int margem){

	if( margem == ESQUERDA)
		printf("Passageiro %d está atravessando da margem esquerda para a margem direita do rio.\n",pid);
	else
		printf("Passageiro %d está atravessando da margem direita para a margem esquerda do rio.\n",pid);
}

int main(int argc, char *argv[]){
	int lado;
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

	if ( (sem_id = createSPH(6)) == -1)
		sem_id = getIdSPH();
	/* Caso os semáforos não existiam, estes foram criados na chamada da função createSPH e serão inicializados abaixo */
  else{
		srand (time(NULL));
		lado = rand() % 2; /* Margem "aleatória" */
		setValSPH(sem_id, ME, lado); 
		setValSPH(sem_id, MD, (1-lado));
		setValSPH(sem_id, EMBARCA, 3);
		setValSPH(sem_id, ATRAVESSA, 3);
		setValSPH(sem_id, DESEMBARCA, 3);
		setValSPH(sem_id, MUTEX, 1);
	}

	embarca(partida);
	if(desistiu == 0){
		atravessa(partida);
		desembarca(partida);
	}
	printf("Passageiro %d saiu do pier.\n",pid);  
	exit(0);
}

