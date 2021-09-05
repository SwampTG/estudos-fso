#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <memory.h>

/** Infos das Funções
 * 
 * FORK:
 * pid_t fork(void);
 * fork cria duplicata em novo processo,
 * pai e filho em regiões de memória diferentes
 * a única diferença entre os processos são seu
 * pid e ppid(do pai)
 * 
 * IMPORTANTE - a utilização de recursos e o tempo 
 * de cpu são resetados para zero no filho!!!
 * 
 * O filho também não herda timers de seus pais
 * 
 * O sinal de término do filho é sempre SIGCHLD
 * ----------------
 * WAIT:
 * pid_t wait(int *wstatus);
 * pid_t waitpid(pid_t pid,  int  *wstatus, int options);
 * int  waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
 * 
 * wait -> espera qualuque filho mudar de estado(terminar, ser
 *   parado por um sinal ou continuado por um sinal ) -> retorna id do processo que terminou
 * 
 * waitpid -> espera pelo pid especificado e, por padrão, só 
 *      será liberado quando o filho termina, mas o arg options
 *      modificar esse comportamento.
 *      pid = -1 -> espera qq filho
 *          = 0 -> espera qq filho com o mesmo group pid do pai
 *          > 0 -> espera o filho especificado
 *      podemos checar os valores de wstatus com MACROS:
 *          WIFEXITED(wstatus) - > retorna true se o filho terminou normalmetne
 *         IMPORTANTE:  WEXITSTATUS(wstatus) -> retorna valor de saída do filho
 *                          usada em conjunto de WIFEXITED
 *          WIFSIGNALED(wstatus) -> retorna true se foi terminada por sinal
 *           WTERMSIG(wstatus)-> retorna sinal que matou. Usado em conjunto com WIFSIGNALED
 * 
 * GETTIMEOFDAY:
 * int gettimeofday(struct timeval *tv, struct timezone *tz);
 * int settimeofday(const struct timeval *tv, const struct timezone *tz);
 * RETORNAM 0 para sucesso e -1 para falha
 * 
 *      STRUCTS:
 *      struct timeval {
 *              time_t      tv_sec;  -> segundos 
 *              suseconds_t tv_usec; -> microsegundos 
 *          };
 *  se a struct timeval ou timezone for NULL, a estrutura não é retornada
 *  timezone deve ser NULL
 * 
 * STRERROR:
 * char *strerror(int errnum); -> retorna a descrição do erro representado por errnum
 *
 * EXECL:
 * int execl(const char *path, const char *arg, ...(char  *) NULL );
 * int execv(const char *path, char *const argv[]);
 * Substitui o processo atual por uma nova imagem de processo.
 * 
 * Primeiro argumento é o arquivo que será executado(camiho),
 * pode-se ter n argumentos distintos. Para identificar o final dos argumentos, DEVEMOS
 * declarar um ponteiro nulo (void *) NULL como último argumento
 * Para execv, argv é a lista de possíveis argumentos disponíveis para o novo programa
 * 
 * as funções exec só retornam se um erro ocorreu e errno é setado para indicar o erro
 * EXECL precisa do PATH completo para funcionar
 **/

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

// TESTES
/* Status de saída do filho */
int wstatus;
int main(void)
{
    /* Caminho do executável(binário) */
    char caminho[251], programa[251];

    // char *arg1 = "-lh";

    /* argumentos do executável Ex: ls -a /home/adrian/UnB 
    -> ls é o comando e todo o resto argumentos */
    char arg1[251];

    /* string como um todo*/
    char frase[502];

    gettimeofday(&tprograma_i, NULL);
    while(scanf(" %[^\n]", frase) != EOF)
    {
        // scanf("\n");
        int k = 0;
        int i = 0, j = 0, z=0;

        while(frase[k]!='\0')
            caminho[i++] = frase[k++];

        k++;

        while(frase[k]!='\0')
            arg1[j++] = frase[k++];

        while(frase[k]!='\0')
            --k;
        while(frase[k]!='/')
            --k;
        k++;
        while(frase[k]!='\0')
            programa[z++] = frase[k++];
        
        //TESTE
        // printf("%s\n", programa);


        /* Fork é necessário pois o exec transfere a imagem do processo para a 
        imagem do binário(Processo deixa de existir)
        */
        pid_t filho = fork();

        /*//TESTE
        if(filho == 0) printf("Fui criado, sou %d e meu pai é %d\n", getpid(), getppid());
        */

        /* Captura tempo antes da execução*/
        if(filho != 0)
            gettimeofday(&antes, NULL);
        /* Filho executa o binário */
        if(filho == 0) 
        {
            //teste

            /* Executa o binário, deve terminar com NULL
            o caminho do binário, , por conveção*/
            if(execl(caminho, programa, /*arg2,*/ arg1, (char *) NULL) == -1) // substitui o processo
                exit(errno);
            /*Nada abaixo será executado!!*/
            // wait(&wstatus);
            // printf("errno: %s -- exit status filho: %d \n", strerror(errno), WEXITSTATUS(wstatus)); 
        } 
        // Espera retorno do filho
        wait(&wstatus);

        /* Captura tempo depois da execução */
        gettimeofday(&depois, NULL);

        /* //teste 
        printf("Antes: %ld sec e %ld micsec\n", antes.tv_sec, antes.tv_usec);
        printf("Depois: %ld sec e %ld micsec\n", depois.tv_sec, depois.tv_usec);
        */

        /* Intervalo em segundos */
        time_t interval_sec = depois.tv_sec - antes.tv_sec;

        /* INtervalo quebrado */
        long double interval_mic = (long double) abs((depois.tv_usec - antes.tv_usec))/MICTOSEC ;
        /** //teste 
        //  printf("Intervalo quebrado: %lf\n", interval_mic);
        */

        /* Intervalo de execução do comando */
        long double decorrido = ((long double) interval_sec) + interval_mic;

        /* Se o filho retornou */
        if(WIFEXITED(wstatus))
        {
            // Se teve status de ero, printa o erro
            if(WEXITSTATUS(wstatus)!=0)
            {
                printf("> Erro: %s\n", strerror(WEXITSTATUS(wstatus)));
            }
            printf("> Demorou %.1Lf segundos, retornou %d\n", decorrido, WEXITSTATUS(wstatus));
        }

        memset((void *) programa, 0, sizeof(char)*251);
        memset((void *) arg1, 0, sizeof(char)*251);
        memset((void *) caminho, 0, sizeof(char)*251);

    }

    gettimeofday(&tprograma_f, NULL);
    time_t interval_sec = tprograma_f.tv_sec - tprograma_i.tv_sec;
    long double interval_mic = (long double) abs((tprograma_f.tv_usec - tprograma_i.tv_usec))/MICTOSEC;

    // printf("Intervalo quebrado: %lf\n", interval_mic);

    t_total = ((long double) interval_sec) + interval_mic;
    printf(">> O tempo total foi de %.1Lf segundos\n", t_total);
    return 0;
}