//Programa que realiza system calls para a remoção de um diretório vazio

//Programa que exemplifica a utilização da system call rmdir(const char *path), que possui como único argumento o local do diretório vazio a ser removido

//system calls envolvidas: execve(); brk(); access(); openat(); fstat(); mmap(); close(); read(); mprotect(); arch_prctl(); munmap(); write(); read(); rmdir(); lseek(); exit_group();

//Chamada das bibliotecas
#include <stdio.h>
#include <unistd.h>

int main(){
	char nome[50];//definição da string que ira conter o endereço do diretório a ser removido
	printf("Endereço do diretório a ser removido:");
	scanf("%s", nome);//coleta do endereço
	int s=rmdir(nome);//executará a system call rmdir e s receberá sua saída, sendo igual a 0 se o diretório for removido e igual a -1 se a remoção não for possível
	if(s==0){
		printf("Diretorio removido\n");
	}else{
		printf("Não foi possível remover diretório, diretório não existe ou não está vazio\n");
	}
	return 0;
}