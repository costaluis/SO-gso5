//Programa que realiza system calls de gerenciamento de memória e arquivos.
//mmap(), mprotect(), brk() / open(),  write(), close()

//Chamadas de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definicao do tamanho maximo da string
#define TAM_VET 100

int main(int argc, char * argv[]){
    //Definicao do ponteiro para arquivo
    FILE * p;

    //Abertura do arquivo
    p = fopen("arquivos.txt","wt");

    //Alocacao da string na memoria
    char * texto = malloc(TAM_VET * sizeof(char));

    //Atribui a string
    strcpy(texto,"Saida do programa que executa system calls de gerenciamento de memoria e arquivos.");
    
    //Escreve no arquivo
    fwrite(texto,strlen(texto),1,p);

    //Liberacao da memoria
    fclose(p);
    free(texto);

    //Retorno da funcao main
    return 0;

}