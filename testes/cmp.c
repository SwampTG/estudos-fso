#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char p[12];
    char *p2 = malloc(sizeof(char) * 12);

    scanf("%s", p);

    p2 = "Conv1\0";

    printf("Usando strncmp:\n");
    if(strncmp(p, p2, 6)==0) printf("Certo\n");
    else printf("Errado\n");

    printf("Usando ifs:\n");
    if(*p == *p2)
        printf("Certo\n");
    else 
        printf("Errado\n");
    
    return 0;
}