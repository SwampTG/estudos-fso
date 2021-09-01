// SIGUSR1 - câmera do Igor3k
// SIGUSR2 - câmera do Monark
// SIGINT - câmera do Convidado 1
// SIGTERM - câmera do Convidado 2
// SIGALRM - câmera geral

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CERTO() {printf("Certo\n");}
#define ERRADO() {printf("Erro\n");}

char pessoa[12];

void check_pessoa(char *p, int n)
{
    if(strncmp(p, pessoa, n)==0)
    {
        CERTO();
    }
    else 
        ERRADO(); 
        
        
}
void tratador(int sig)
{
    sleep(0.5);

    if (scanf("%s", pessoa) != EOF)
    {
        char *p = malloc(sizeof(char) * 12);

        if(sig == SIGUSR1)
        {
            p = "Igor3k\0";
            check_pessoa(p, 6);
        }
        else if(sig == SIGUSR2)
        {
            p = "Monark\0";
            check_pessoa(p, 6);
        }
        else if(sig == SIGINT)
        {
            p = "Con1\0";
            check_pessoa(p, 4);
        }
        else if(sig == SIGTERM)
        {
            p = "Con2\0";
            check_pessoa(p, 4);
        }
        else if(sig == SIGALRM)
        {
            p = "Silencio\0";
            check_pessoa(p, 8);
        }
        sleep(0.5);
        //free(p);
    }
    else exit(0);

}

int main(void)
{
    signal(SIGUSR1, tratador);
    signal(SIGUSR2, tratador);
    signal(SIGINT, tratador);
    signal(SIGTERM, tratador);
    signal(SIGALRM, tratador);

    while(1) pause();

    return 0;
}
