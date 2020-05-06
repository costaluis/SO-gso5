//Programa que realiza system calls para a remoção de um diretório vazio

//Programa que exemplifica a utilização da system call rmdir(const char *path), que possui como único argumento o local do diretório vazio a ser removido

//system calls envolvidas(tempo em microssegundos):read(tempo de input do usuário);execve(352);write(196);rmdir(133);mmap(88);mprotect(81);fstat(55);access(48);brk(41);munmap(38);openat(37);close(25);lseek(18);arch_prctl(12);

//Chamada das bibliotecas
#include <stdio.h>
#include <unistd.h>

int main(){
	//definição da string que ira conter o endereço do diretório a ser removido
	char nome[50];
	printf("Endereço do diretório a ser removido:");
	//coleta do endereço (ex:/home/gso05/pasta)
	scanf("%s", nome);
	//executará a system call rmdir e s receberá sua saída, sendo igual a 0 se o diretório for removido e igual a -1 se a remoção não for possível
	int s=rmdir(nome);
	if(s==0){
		printf("Diretorio removido\n");
	}else{
		printf("Não foi possível remover diretório, diretório não existe ou não está vazio\n");
	}
	return 0;
}
