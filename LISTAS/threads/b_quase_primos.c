#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define INI 508079

typedef struct sthread
{
    pthread_t tid;
    int num;
    int pos;
} sthread;

pthread_mutex_t mutex;
sthread *th;
// int k = 0;

void * procura_quase_primo(void *threadv)
{
    sthread * t = (sthread *) threadv;
    int proc = t->num;
    // printf("proc: -> %d\n", proc);

    pthread_mutex_lock(&mutex);

    if(proc <= INI) 
    {
        // pthread_mutex_lock(&mutex);

        th[t->pos].num = INI;
        // k++;

        pthread_mutex_unlock(&mutex);
    }
    else
    {
        // pthread_mutex_lock(&mutex);
        long qprimo_maluco = proc;
        while(qprimo_maluco++)
        {
            int check = 0;
            for(int k = 2; k <= 10; k++)
            {
                if(!(qprimo_maluco % k))
                    check = 1;
            }

            if(check) continue;

            double lim = sqrt(qprimo_maluco);
            // printf("lim >>> %f\n", lim);

            int count = 0;
            for(int j = 11; j <= lim; j++)
            {
                if(count >= 11)
                {
                    // pthread_mutex_unlock(&mutex);
                    break;
                }
                
                if(qprimo_maluco % j == 0)
                    count++;
            }

            if(count < 11)
            {
                continue;
            }
            else
            {
                // printf("%ld\n", qprimo_maluco);
                // pthread_mutex_lock(&mutex);

                th[t->pos].num = qprimo_maluco;
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

    int x, y, i = 0;
    while(1)
    {
        if(size % 2 == 1 && n <= 1)
        {
            scanf("%d", &x);
            th[i].num = x;
            th[i].pos = i;

            pthread_create(&th[i].tid, NULL, &procura_quase_primo, (void *) &th[i]);

            pthread_join(th[i].tid, NULL);
            i++;
            break;
        }
        else
        {
            scanf("%d", &x);
            scanf("%d", &y);

            th[i].num = x;
            th[i].pos = i;
            th[i+1].num = y;
            th[i+1].pos = i+1;

            // printf("th[%d]: pos = %d; num = %d\n........\n", i, th[i].pos, th[i].num);
            // printf("th[%d]: pos = %d; num = %d\n........\n", i+1, th[i+1].pos, th[i+1].num);

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

    // printf("teste th.num = %d, th.pos = %d", th[0].num, th[0].pos);

    for(int i = 0; i < size; i++)
        printf("%d\n", th[i].num);
    
    pthread_exit(NULL);
    return 0;
}
