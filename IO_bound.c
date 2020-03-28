//Algoritmo IO-bound que inicialmente escreve "sistemas_operacionais__" em um arquivo e em seguida entra em um loop de escrever no final do arquivo tudo o que já foi escrito no arquivo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	//"p" será o ponteiro para o arquivo que será lido e escrito
	FILE * p;
	int i, tamanho = 24;
	//abertura do arquivo para escrita
	p = fopen("IO.txt", "w");
	//declaração de "texto", que conterá a frase a ser escrita
	char * texto;
	//alocação de "texto"
	texto = (char *)malloc(tamanho);
	//armazenar a frase "sistemas_operacionais" na string "texto" 
	strcpy(texto, "sistemas_operacionais__");
	//escrevendo no arquivo
	fputs(texto, p);
	fclose(p);

	for(i=0; i<25; i++){
		//abertura do arquivo para leitura
		p = fopen("IO.txt", "r");
		//armazenar o que já foi escrito no arquivo na string "texto"
		fgets(texto, tamanho, p);
		fclose(p);
		//abertura do arquivo para escrita no final do arquivo
		p = fopen("IO.txt", "a");
		//escrevendo no arquivo
		fputs(texto, p);
		fclose(p);
		// a frase do arquivo foi duplicada, logo, é necessário duplicar o tamanho da string que lê o arquivo
		tamanho = tamanho*2;
		//realoção de "texto" com o novo tamanho
		texto = (char *)realloc(texto, tamanho);
	}

	free(texto);
	return(0);
}
