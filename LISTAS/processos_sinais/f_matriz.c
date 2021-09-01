#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

struct matriz
{
    int i;
    int f;
    int tam;
    int sig;
};

int sig1= 0;
int sig2 = 0;

int matrix1[2000][2000];
int matrix2[2000][2000];
int matrix3[2000][2000];
// matrix 1 é o melhor

void trata(int sig)
{
    return;
}

void tratasig1(int sig)
{
    sig1++;
}

void tratasig2(int sig)
{
    sig2++;
}

void *multiplica(void *l)
{
    signal(SIGUSR1, trata); // somente para não morrer (pai irá mandar sigusr 1)

    for(int i = ((struct matriz *) l)->i; i < ((struct matriz *) l)->f; i++)
    {
        for(int j = 0; j < ((struct matriz *) l)->tam ; j++)
        {
            for(int k = 0; k < ((struct matriz *) l)->tam; k++)
            {
                matrix3[i][j] += matrix2[i][k] * matrix2[k][j];
            }
        }
    }

    int r = 0;
    kill(getppid(), ((struct matriz *) l)->sig); // avisa o pai

    pause(); // espera sinal do pai

    //imprime
    for(int i = ((struct matriz *) l)->i; i < ((struct matriz *) l)->f; i++)
    {
        printf("%d", matrix3[i][0]);
        for(int j = 1; j < ((struct matriz *) l)->tam; j++)
           printf(" %d", matrix3[i][0]); 
        printf("\n");
    }
    kill(getppid(), ((struct matriz *) l)->sig); // avisa o pai de novo;
    exit(0); //morre
}

int main(void)
{
    signal(SIGUSR1, tratasig1);
    signal(SIGUSR2, tratasig2);
    
    int n;
    scanf("%d", &n);
    // lendo matrizes
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            scanf("%d", &matrix1[i][j]);
            matrix3[i][j] = 0; // zerando a matrix do somatório
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            scanf("%d", &matrix2[i][j]);
        }
    }
    //dividindo matrizes

    int processos = 2;
    int div = n/processos;
    int res = n % processos; // se a matriz tiver tam ímpar

    pid_t ids[processos];
    int sigs[2];
    sigs[0] = SIGUSR1; // primeira matriz trata SIUSR1
    sigs[1] = SIGUSR2; // segunda matriz

    for(int i = 0; i < processos; i++)
    {
        struct matriz *aux = malloc(sizeof(struct matriz));

        aux->i = i*div;
        aux->sig = sigs[i];
        //verificando tam da matriz
        if(i == processos-1)
            aux->f = div * (res * i + 1); //primeiro irá calcular uma linha a mais caso tam não seja par
        else
            aux->f = div * (i+1);

        aux->tam = n;
        ids[i] = fork(); // processos em paralelo
        if( ids[i] == 0) // processo filho
            multiplica(aux);
    }

    // pause(); // espera sinal do filho 1
    // garantir que os dois processos filhos executaram:
    while(sig1 == 0 || sig2 == 0) // quando envia sinal, encrementa
        pause(); // espera os 2 processos chamarem a função e um deles enviar o sinal

    kill(ids[0], SIGUSR1); // manda sinal para o filho imprimir
    pause(); // espera sinal do filho 2
    kill(ids[1], SIGUSR1);


    wait(NULL); // sinaliza retorno do filho 1
    wait(NULL); // sinaliza retorno do filho 2;
    return 0;
}
