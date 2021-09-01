#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

typedef struct sthread
{
    pthread_t tid;
    int seed;
    int proc;

} sthread;

sthread threads[NTHREADS];

int main(void)
{/**Teste
    int a;
    unsigned int seed = 57; 
    for(int i = 0; i<100000; i++){
        a = rand_r(&seed) % 8;
    }

    printf("%d\n", a);
**/

    int mi, mf;
    int proc;

    while(scanf("%d %d %d", &mi, &mf, &proc) == 3)
    {
        for(int i = mi; i <= mf-2; i+=4)
            for(int j = 0; j < NTHREADS; j++)
            {
                pthread_create(&threads[j].tid, NULL, );
            }

        pthread_exit(NULL);
    }

    return 0;
}