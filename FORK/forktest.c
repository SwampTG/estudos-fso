#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
	pid_t p;
	p = fork();
	p = fork();
	printf("Olá Mundo\n");
	sleep(50);
}
