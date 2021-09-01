#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    pid_t p = 0;
    pid_t p2 = 0;

    for(int i = 0; i<2; i++) {
        p = fork();

        if (p == 0)
            p2 = fork();
        if (p2 == 0)
            printf("I am %d, My dad is : %d. I see dead forks...\n", getpid(), getppid());        
    }

    exit(0);

    return 0;
}
