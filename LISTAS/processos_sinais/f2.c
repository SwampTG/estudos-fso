#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int n;
int f1_end;
int f2_end;

void paiimortal(int signal)
{
    return;
}

void mult_matriz(int *matrix, int *matrix2, int ini, int fim)
{

}

int main(void)
{
    signal(SIGUSR1, paiimortal);
    scanf("%d", &n);
    int matrix1[n][n];
    int matrix2[n][n];

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", matrix1[i][j]);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", matrix2[i][j]);

    int n1, n2;

    n1 = n/2;
    n2 = n;
    
    pid_t f1, f2;

    f1 = fork();
    if(f1 != 0) f2 = fork();

    if(f1 == 0)
    {
        mult_matriz(matrix1, matrix2, 0, n1);
    }
    if(f2 == 0)
    {
        mult_matriz(matrix1, matrix2, n1, n2);
    }


}
