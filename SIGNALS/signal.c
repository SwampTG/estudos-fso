#include <signal.h>

void trataSinal (int sig) //função de tratamento de sinal
{
    return;
}

int main(void) 
{
    signal(SIGINT, trataSinal(SIGINT)); // (SIGINT, função_handler)

    kill(getppid(), SIGKILL); // Para envio de sinais (pid_t, int signal)
    return 0;
}

