#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void vemComFilho();

void vemComPai() 
{
	printf("%d: Sou o pai!\n", getpid());
	pid_t p;
	p = fork();
	if (p == 0)
		vemComFilho();
	int ret;
	p = wait(&ret); //Espera o filho morrer, retorna o pid do processo que ele espera
	printf("Filho de nome: %d terminou com %d, vou vazar\n", p, WEXITSTATUS(ret));
	p = wait(&ret);
	printf("Filho de nome: %d terminou com %d, vou vazar\n", p, WEXITSTATUS(ret));
	exit(0);
	// exit(NULL); //Criamos um filho sem pai, INIT assume
}

void vemComFilho()
{
	printf("%d: Sou o filho, e meu pai é: %d\n", getpid(), getppid());
	sleep(1);

	printf("%d: Sou o filho, e meu pai é: %d\n", getpid(), getppid());
	exit(3); // Se sairmos sem utilizar o retorno do processo no pai, criamos um ZUMBI!!
}

int main(void) 
{
	pid_t p = fork();
	if (p>0)
			vemComPai();
	else if (p==0)
			vemComFilho();
	else printf("fork falhou\n");

	sleep(50);
}
