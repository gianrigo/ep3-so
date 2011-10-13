#include <stdio.h>
#include <stdlib.h>
#define ESQUERDA 0
#define DIREITA 1

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
	int pid;

	if( margem == 0)
		printf("O processo %d está atravessando da margem esquerda para a margem direita do rio.\n",pid);
	else
		printf("O processo %d está atravessando da margem direita para a margem esquerda do rio.\n",pid);
}

int main(int argc, char *argv[]){
	int partida;

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
	embarca(partida);
	atravessa(partida);
	desembarca(partida);
	/* imprime passageiro saiu do pier */
	exit(0);
}

