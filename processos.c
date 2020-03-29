//Programa que realiza system calls de gerenciamento de processos.
//clone(), wait4(), exit_group()

//A syscall clone() cria um novo processo (“filho”) idêntico ao “pai”, com diferenças apenas no PID e no espaço de endereçamento de memória. Age como um wrapper da syscall “fork()”, utilizada explicitamente no código. 

//A função waitpid(), utilizada explicitamente no código, é implementada como uma função wrapper que chama wait4(). Sua funcionalidade é parar a execução até que um processo especificado mude de estado.

//A syscall kill() é utilizada para enviar um sinal qualquer à um processo ou grupo de processos.

//system calls envolvidas(tempo em microssegundos): wait4(634); write(420); execve(130); clone(53); mmap(41); mprotect(39); access(23); kill(22); fstat(19); brk(19); openat(18); munmap(17); close(12); read(7); arch_prctl(5)

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
        //Quando o signal eh 0, o comando nao envia o signal, mas a execucao ocorre, checando a existencia do processo.
        kill(WUNTRACED,0);
    }
    
    return 0;
}