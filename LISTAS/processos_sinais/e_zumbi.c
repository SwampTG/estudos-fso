#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>

void trata(int sig)
{
    sleep(0.1);
    return;
}

int main(void)
{
    signal(SIGUSR1, trata);
    signal(SIGUSR2, trata);

    pause();

    pid_t pid = fork();

    if(0 == pid)
    {
        exit(0);
    }

    pause();

    wait(NULL);


    pause();
    exit(0);


}