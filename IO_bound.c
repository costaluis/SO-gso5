//Algoritmo IO-bound

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE * p;
	int i, tamanho = 24;
	p = fopen("IO.txt", "w");
	char * texto;
	texto = (char *)malloc(tamanho);
	strcpy(texto, "sistemas_operacionais__");
	fputs(texto, p);
	fclose(p);

	for(i=0; i<25; i++){
		p = fopen("IO.txt", "r");
		fgets(texto, tamanho, p);
		fclose(p);
		p = fopen("IO.txt", "a");
		fputs(texto, p);
		fclose(p);
		tamanho = tamanho*2;
		texto = (char *)realloc(texto, tamanho);
	}

	free(texto);
	return(0);
}
