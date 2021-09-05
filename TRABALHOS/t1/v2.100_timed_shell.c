#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <memory.h>

#define MICTOSEC (1000000)

/** struct timeval
 * Struct para recuperar o tempo atual
 * podemos fazer uma subtração para intervalo de tempo decorrido
 * converter usec para sec: microsec/1000000
 * somar com sec e printar %.1f (uma casa de precisão)
**/
struct timeval antes, depois, tprograma_f, tprograma_i;
long double t_total = 0.0;
long double t_programa = 0.0;

/* Caminho do executável(binário) */
char caminho[251];

// char *arg1 = "-lh";

/* argumentos do executável Ex: ls -a /home/adrian/UnB 
-> ls é o comando e todo o resto argumentos */
char arg1[251];

char nome[251];

// TESTES
/* Status de saída do filho */
int wstatus;
int main(void)
{
    memset((void *) caminho, 0, sizeof(char)*251);
    memset((void *) arg1, 0, sizeof(char)*251);

    gettimeofday(&tprograma_i, NULL);
    while(scanf("%s %s", caminho, arg1) == 2)
    {
        char * bzero = strchr(caminho, '\0');
        while(*bzero != '/')
            --bzero;
        int aux = 0;
        while(*bzero != '\0')
        {
            nome[aux++] = *bzero;
            ++bzero;
        }
        nome[aux] = '\0';

        // TEste:
        // printf("%s %s", caminho, arg1);
        char * args [/*3*/ 3] = {/*caminho,*/nome, arg1, NULL};

        /* Captura tempo antes da execução*/
        gettimeofday(&antes, NULL);

        /* Fork é necessário pois o exec transfere a imagem do processo para a 
        imagem do binário(Processo deixa de existir)
        */
       
        pid_t filho = fork();

        /*//TESTE
        if(filho == 0) printf("Fui criado, sou %d e meu pai é %d\n", getpid(), getppid());
        */
       
        /* Filho executa o binário */
        if(filho == 0) 
        {
            //teste
            printf("Fui criado, sou %d e meu pai é %d\n", getpid(), getppid());
            printf("Antes: %ld sec e %ld micsec\n", antes.tv_sec, antes.tv_usec);
            // fflush(stdout);

            /* Executa o binário, deve terminar com NULL
            o caminho do binário, , por conveção*/
            setbuf(stdout, NULL);
            if(execv(caminho, args) == -1) // substitui o processo
                exit(errno);
            /*Nada abaixo será executado!!*/
            // wait(&wstatus);
            // printf("errno: %s -- exit status filho: %d \n", strerror(errno), WEXITSTATUS(wstatus)); 
        } 
        else
        {
            // Espera retorno do filho
            filho = wait(&wstatus);
      
            kill(filho, SIGUSR1);
            /* Captura tempo depois da execução */
            gettimeofday(&depois, NULL);

            //teste 
            printf("Depois: %ld sec e %ld micsec\n", depois.tv_sec, depois.tv_usec);/*
            printf("Antes: %ld sec e %ld micsec\n", antes.tv_sec, antes.tv_usec);
            */
            printf("-----------------------------------\n");

            /* Intervalo em segundos */
            time_t interval_sec = depois.tv_sec - antes.tv_sec;

            /* INtervalo quebrado */
            long double interval_mic = (long double) abs((depois.tv_usec - antes.tv_usec))/MICTOSEC ;
             //teste 
            //  printf("Intervalo quebrado: %Lf\n", interval_mic);

            /* Intervalo de execução do comando */
            long double decorrido = ((long double) interval_sec) + interval_mic;

            /* Se o filho retornou */
            if(WIFEXITED(wstatus))
            {
                // Se teve status de ero, printa o erro
                if(WEXITSTATUS(wstatus)!=0)
                {
                    setbuf(stdout, NULL);
                    printf("> Erro: %s\n", strerror(WEXITSTATUS(wstatus)));
                }
                setbuf(stdout, NULL);
                printf("> Demorou %.1Lf segundos, retornou %d\n", decorrido, WEXITSTATUS(wstatus));
            }

            // t_total += decorrido;

            memset((void *) caminho, 0, sizeof(char)*251);
            memset((void *) arg1, 0, sizeof(char)*251);
        }

    }

    gettimeofday(&tprograma_f, NULL);
    time_t interval_sec = tprograma_f.tv_sec - tprograma_i.tv_sec;
    long double interval_mic = (long double) abs((tprograma_f.tv_usec - tprograma_i.tv_usec))/MICTOSEC;

    // printf("Intervalo quebrado: %lf\n", interval_mic);

    t_total = ((long double) interval_sec) + interval_mic;
    printf(">> O tempo total foi de %.1Lf segundos\n", t_total);
    
    return 0;
}