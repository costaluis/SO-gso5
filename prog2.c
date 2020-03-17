//Programa que realiza system calls de gerenciamento de processos.
//clone(), wait4(), exit_group()

//Chamadas das bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argv, char * argc[]){
    //Variavel que armazena o id dos processos
    __pid_t processo;
    //Variavel que armazena o status do processo
    int * status;

    //Cria um processo filho
    processo = fork();
    
    //Se for o processo filho...
    if(processo == 0){
        //Imprime uma contagem de 0-9
        for(int i=0; i<10; i++){
            printf("Filho: %d ",i);
            printf("\n");
        }
    }else
    //Se for o processo pai...
    {
        //Aguarda o processo filho terminar
        waitpid(processo, status, WUNTRACED);
        //Imprime uma contagem de 0-9
        for(int i=0; i<10; i++){
            printf("Pai: %d ",i);
            printf("\n");
        }
    }
    
    return 0;
}