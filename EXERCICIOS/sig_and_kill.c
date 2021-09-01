#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void trata_sinal(int s)
{
    if (s== SIGUSR1) 
    {
        printf("%d: Continua tentando que eu vou te azucrinando...\n", getpid());
    }
    else if (s==SIGALRM) 
    {
        printf("%d: Alerta de TROUXA!\n", getpid());
        alarm(5);
    }
    else if (s==SIGTERM)
        printf("Tá se achando em...\n");
}

void fazcoisadefilho(void)
{
    while(1)
    {
        printf("%d: Pai %d me dá dinheiro... \n", getpid(), getppid());
        sleep(10);
        kill(getppid(), SIGUSR1);
    }
}

int main (void)
{
    signal(SIGUSR1, trata_sinal);
    signal(SIGUSR2, trata_sinal);
    signal(SIGTERM, trata_sinal);
    signal(SIGINT, trata_sinal);
    signal(SIGALRM, trata_sinal);

    alarm(2);

    pid_t p = fork();
    if (p == 0) fazcoisadefilho();

    while(1)
        pause();
    
    return 0;
}