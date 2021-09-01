#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int v1[1000000000];
int v2[1000000000];

// colocar tudo em struct facilita os args da thread create
struct thread
{
    pthread_t tid; // id da thread
    int *v;
    int tam;
};

int compare (const void * a, const void * b); // parâmetro do qsort
void *func(void * a);
void ordena(int *v, int n);


int main(void)
{
    struct thread t1, t2; // 2 threads
    int n ; // tamanho
    scanf("%d", &n);

    t1.v = malloc(sizeof(int)*n);
    t2.v = malloc(sizeof(int)*n);

    t1.tam = n;
    t2.tam = n;

    for(int i = 0; i<n; i++)
    {
        int x;
        scanf("%d", &x);        
        t1.v[i] = x;
    }
    for(int i = 0; i<n; i++)
    {
        int x;
        scanf("%d", &x);        
        t2.v[i] = x;
    }
    
    pthread_create(&t1.tid, NULL, &func, (void *)&t1); //converter args para ponteiro void
    pthread_create(&t2.tid, NULL, &func, (void *)&t2); // a função reconverte para o que precisamos
    // "wait" das threads:
    pthread_join(t1.tid, NULL);
    pthread_join(t2.tid, NULL);

    for(int i = 0; i < n; i++)
    {
        if(t1.v[i] == t2.v[i])
            if(i == n-1)
            {
                printf("Mesmos elementos\n");
                return 0;
            }     
    }

    printf("Diferentes\n");

    return 0;
}

int compare (const void * a, const void * b)
{
    return (*(int *) a - *(int *) b); //retorna 0, positivo ou negativo
}

void ordena(int *v, int n)
{
    qsort(v, n, sizeof(int), &compare);
}

void *func(void * a)
{
    struct thread * dado = (struct thread *) a;
    ordena(dado->v, dado->tam);
}