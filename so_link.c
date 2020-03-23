//Programa que realiza system calls para a criação de uma nova entrada apontando para um arquivo ja existente

//Programa que exemplifica a utilização da system call link(const char *path1, const char *path2), que possui como primeiro argumento a localização do arquivo a receber uma nova entrada,
// e segundo argumento o local que conterá a nova entrada

//system calls envolvidas: execve(); brk(); access(); openat(); fstat(); mmap(); close(); read(); mprotect(); arch_prctl(); munmap(); write(); read(); link(); lseek(); exit_group();

//Chamadas de bibliotecas
#include <stdio.h>
#include <unistd.h>

int main(){
	char nome1[50], nome2[50];// definição da string nome1 que deverá conter o endereço do arquivo que receberá uma nova entrada, e da string nome2 que deverá conter o endereço da nova entrada
	printf("Endereço o arquivo que deverá possuir uma nova entrada:");
	scanf("%s", nome1);//recebe o endereço do arquivo que receberá uma nova entrada
	printf("Endereço da nova entrada:");
	scanf("%s", nome2);//recebe o endereço da nova entrada
	int s=link(nome1,nome2);//executará a system call link e s receberá sua saída, sendo 0 quando o link conseguir ser criado e -1 quando não
	if(s==0){
		printf("Nova entrada de %s criada em %s \n", nome1, nome2);
	}else{
		printf("Não foi possível criar nova entrada de %s \n", nome1);
	}
	return 0;
}