//Programa que realiza system calls para a criação de um diretorio

//Programa que exemplifica a utilização da system call mkdir(const char *pathname, mode_t mode), que possui como primeiro argumento o local que irá conter o novo diretório,
//e segundo argumento o modo, que determina a permissão de acesso ao diretório.

//system calls envolvidas: execve(); brk(); access(); openat(); fstat(); mmap(); close(); read(); mprotect(); arch_prctl(); munmap(); write(); read(); mkdir(); lseek(); exit_group();

//Chamadas de bibliotecas
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
	char nome[50];//definição da string que ira conter o endereço onde sera criado o novo diretorio
	printf("Endereço do diretorio a ser criado:");
	scanf("%s", nome);//coleta do endereço
	int s=mkdir(nome, S_IRWXU|S_IRWXG|S_IRWXO);//executará a system call mkdir e s receberá sua saída, sendo igual a -1 se o diretório nao for criado e igual a 0 se for criado
	if(s==0){
		printf("Novo diretório criado\n");
	}else{
		printf("Erro ao criar novo diretorio\n");
	}
	return 0;
}
