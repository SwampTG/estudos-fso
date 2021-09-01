// Criar função que processa sinais
// associar aos sinais SIGUSR1, SIGUSR2, SIGTERM e SIGINT
// criar loop para os sinais
// imprimir cód dos sinais enviados
// caso tenha a ordem correta, imprimir senha acionada
// caso tenha a ordem correta dos sinais, imprimir tchau e sair.
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define PRINT_CONF() printf("Senha acionada\n")
#define TCHAU() printf("Tchau\n")

int aux = 0;

void trata_sinal(int s);

int main(void)
{
    // Associação de sinais:
    signal(SIGINT, trata_sinal);
    signal(SIGTERM, trata_sinal);
    signal(SIGUSR1, trata_sinal);
    signal(SIGUSR2, trata_sinal);

    // alarm(5);
    // Loop para sinais
    while(1)
        pause(); // Wait para o envio de sinais!!!

    return 0;
}

void trata_sinal(int s)
{
    printf("Recebi %d\n", s);

    if (s== SIGINT && aux == 0) aux++; // Caso que vai de 0 para 1
    else if (s == SIGINT) aux = 1; // Não pode voltar para o zero, se não a ordem é alterada
    else if (s == SIGUSR2 && aux==1) aux++;
    else if (s == SIGTERM && aux==2) 
    {
        aux++;
        PRINT_CONF();
    }
    else if (s == SIGUSR1 && aux == 3)
    {
        TCHAU();
        exit(0);
    }
    else aux = 0;
        //printf("%d: Continua tentando que eu vou te azucrinando...\n", getpid());

}
