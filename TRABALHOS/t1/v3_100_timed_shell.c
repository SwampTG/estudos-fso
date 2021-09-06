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
/**
 * antes e depois do binário / antes depois do programa
*/
struct timeval antes, depois, tprograma_f, tprograma_i;
long double t_total = 0.0;
long double t_programa = 0.0;

/* Caminho do executável(binário) */
char caminho[251];

/* argumentos do executável Ex: ls -a /home/adrian/UnB 
-> ls é o comando e todo o resto argumentos */
char arg1[251];

/* binnome do binário */
char binnome[251];

// TESTES
/* Status de saída do filho */
int wstatus;
int main(void)
{
    pid_t bashpid = getppid();

    memset((void *) caminho, 0, sizeof(char)*251);
    memset((void *) arg1, 0, sizeof(char)*251);
    memset((void *) binnome, 0, sizeof(char)*251);

    gettimeofday(&tprograma_i, NULL);
    while(scanf("%s %s", caminho, arg1) == 2)
    {
        /**
         * @brief Obter nome do binário:
         * Última string após '/';
         * Termina em 0
         * 
         */
        char * bzero = strchr(caminho, '\0');
        while(*bzero != '/')
            --bzero;
        ++bzero;
        int aux = 0;
        while(*bzero != '\0')
        {
            binnome[aux++] = *bzero;
            ++bzero;
        }
        binnome[aux] = '\0';

        // TEste:
        // printf("%s %s", caminho, arg1);
        char * args [3] = {/*caminho,*/binnome, arg1, NULL};

        /* Fork é necessário pois o exec transfere a imagem do processo para a 
        imagem do binário(Processo deixa de existir)
        */
        /* Filho executa o binário */
        if(getppid() == bashpid)
        {
            pid_t filho, wfilho; 
            filho = fork();
       
        /* Captura tempo antes da execução*/
            gettimeofday(&antes, NULL);
            if(filho == 0) 
            {
                /*//teste 
                printf("Fui criado, sou %d e meu pai é %d\n", getpid(), getppid());
                */

                /* 
                printf("Antes: %ld sec e %ld micsec\n", antes.tv_sec, antes.tv_usec);
                
                printf("caminho: %s - ", caminho);
                for(int i = 0; i < 2; i++)
                {
                    printf(" arg[%d]: %s -", i+1, args[i]);
                }
                printf("\n");
                */

                /* Executa o binário, deve terminar com NULL
                o nome do binário é o primeiro argumento em argv, por conveção*/
                setbuf(stdout, NULL);
                execvp(caminho, args); // substitui o processo
                _exit(errno);

                /*Nada abaixo será executado!!*/
                // wait(&wstatus);
                // printf("errno: %s -- exit status filho: %d \n", strerror(errno), WEXITSTATUS(wstatus)); 
            } 
            else
            {
                // Espera retorno do filho
                do 
                {
                    wfilho = waitpid(filho, &wstatus, WUNTRACED);
                    /* Captura tempo depois da execução */
                    gettimeofday(&depois, NULL);
                }
                while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
                
                /* Intervalo de execução do comando */
                long double decorrido = ((long double) ((depois.tv_sec * MICTOSEC + depois.tv_usec) - 
                                            (antes.tv_sec * MICTOSEC + antes.tv_usec))) / MICTOSEC;

                /* //teste 
                printf("Depois: %ld sec e %ld micsec\n", depois.tv_sec, depois.tv_usec);/*
                printf("Antes: %ld sec e %ld micsec\n", antes.tv_sec, antes.tv_usec);
                */

                /* Se o filho retornou */
                if(WIFEXITED(wstatus))
                {
                    // Se teve status de ero, printa o erro
                    if(WEXITSTATUS(wstatus) > 1)
                    {
                        setbuf(stdout, NULL);
                        printf("> Erro: %s\n", strerror(WEXITSTATUS(wstatus)));
                    }
                    else if(WEXITSTATUS(wstatus) == 1 )
                    { 
                        char compara[] = "false\0"; 
                        int check = strncmp(binnome, compara, 5); 

                        if(check) 
                        { 
                            setbuf(stdout, NULL); 
                            printf("> Erro: %s\n", strerror(WEXITSTATUS(wstatus))); 
                        } 
                    }
                    setbuf(stdout, NULL);
                    printf("> Demorou %.1Lf segundos, retornou %d\n", decorrido, WEXITSTATUS(wstatus));
                    //printf("-----------------------------------\n");
                }

                // t_total += decorrido;

                memset((void *) caminho, 0, sizeof(char)*251);
                memset((void *) arg1, 0, sizeof(char)*251);
                memset((void *) binnome, 0, sizeof(char)*251);
            }
        }
        else
            exit(0);
    }

    gettimeofday(&tprograma_f, NULL);
    t_total = ((long double) ((tprograma_f.tv_sec * MICTOSEC + tprograma_f.tv_usec) - 
                (tprograma_i.tv_sec * MICTOSEC + tprograma_i.tv_usec))) / MICTOSEC;

    printf(">> O tempo total foi de %.1Lf segundos\n", t_total);
    
    return 0;
}