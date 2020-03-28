//Programa que realiza system calls para a criação de um diretorio

//Programa que exemplifica a utilização da system call mkdir(const char *pathname, mode_t mode), que possui como primeiro argumento o local que irá conter o novo diretório, e segundo argumento o modo, que determina a permissão de acesso ao diretório.

//system calls envolvidas(tempo em microssegundos): execve(0); brk(8); access(0); openat(0); fstat(7); mmap(36); close(3); read(50); mprotect(28); arch_prctl(4); munmap(16); write(49); read(50); mkdir(282); lseek(13);

//Chamadas de bibliotecas
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
	//definição da string que ira conter o endereço onde sera criado o novo diretorio
	char nome[50];
	printf("Endereço do diretorio a ser criado:");
	//coleta do endereço; deverá ser o endereço de onde a pasta será criada mais o nome da nova pasta (ex:/home/usuariopc/novapasta)
	scanf("%s", nome);
	//executará a system call mkdir e s receberá sua saída, sendo igual a -1 se o diretório nao for criado e igual a 0 se for criado
	int s=mkdir(nome, S_IRWXU|S_IRWXG|S_IRWXO);
	if(s==0){
		printf("Novo diretório criado\n");
	}else{
		printf("Erro ao criar novo diretorio\n");
	}
	return 0;
}
