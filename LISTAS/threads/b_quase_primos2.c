#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define INI 508079

typedef struct sthread
{
    pthread_t tid;
    long num;
    int pos;
} sthread;

pthread_mutex_t mutex;
sthread *th;
int k = 0;

void * procura_quase_primo(void *threadv)
{
    // printf("proc: -> %d\n", proc);

    if(((sthread *) threadv)->num <= INI) 
    {
        pthread_mutex_lock(&mutex);

        th[((sthread *) threadv)->pos].num = INI;
        // th[k].pos = k;
        // k++;

        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    else
    {
        // pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);
        while(((sthread *) threadv)->num++)
        {
            for(int k = 2; k <= 10; k++)
            {
                if(!(((sthread *) threadv)->num % k))
                    continue;
            }
            // pthread_mutex_lock(&mutex);
            double lim = sqrt(((sthread *) threadv)->num);
            // printf("lim >>> %f\n", lim);

            int count = 0;
            for(int j = 11; j <= lim; j++)
            {
                if(count >= 11)
                {
                    // pthread_mutex_unlock(&mutex);
                    break;
                }
                
                if(((sthread *) threadv)->num % j == 0)
                    count++;
            }

            if(count < 11)
            {
                pthread_mutex_unlock(&mutex);
                continue;
            }
            else
            {
                // printf("%ld\n", qprimo_maluco);
                // pthread_mutex_lock(&mutex);

                th[((sthread *) threadv)->pos].num = ((sthread *) threadv)->num;
                // k++;

                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
                break;
            }
        }
    
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);

    int size;
    scanf("%d", &size);

    th = malloc(sizeof(sthread)*size);

    int n =size;

    long x, y;
    int i = 0;
    while(1)
    {
        if(size % 2 == 1 && n <= 1)
        {
            scanf("%ld", &x);
            th[i].num = x;
            th[i].pos = i;

            pthread_create(&th[i].tid, NULL, &procura_quase_primo, (void *) &th[i]);

            pthread_join(th[i].tid, NULL);
            i++;
            break;
        }
        else
        {
            scanf("%ld", &x);
            scanf("%ld", &y);

            th[i].num = x;
            th[i].pos = i;
            th[i+1].num = y;
            th[i+1].pos = i+1;

            pthread_create(&th[i].tid, NULL, &procura_quase_primo, (void *) &th[i]);
            pthread_create(&th[i+1].tid, NULL, &procura_quase_primo, (void *) &th[i+1]);

            pthread_join(th[i].tid, NULL);
            pthread_join(th[i+1].tid, NULL);

            i+=2;
        }

        n-=2;
        if(n <= 0)
            break;
    }

    for(int i = 0; i < size; i++)
        printf("%ld\n", th[i].num);
        // printf("teste th.num = %d, th.pos = %d\n", th[i].num, th[i].pos);

    
    pthread_exit(NULL);
    return 0;
}