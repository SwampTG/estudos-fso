#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>

int aux = 0;

void trata(int sig)
{
    switch (sig)
    {
    case SIGUSR1:
        printf("SIGUSR1 eh para aproveitar mais um pouco\n");
        break;
    
    case SIGALRM:
        if(aux == 2)
        {
            printf("Os incomodados que se mudem, tchau\n");
            exit(0);
        }
        else 
        {
            printf("Ai que sono, quero dormir mais um pouco\n");
            aux++;
        }
        break;

    default:
        break;
    }
}

int main(void)
{
    signal(SIGUSR1, trata);
    signal(SIGALRM, trata);
    
    while(1) pause();

    return 0;
}