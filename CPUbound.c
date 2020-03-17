// Programa exemplo de CPU-bound, em que eh calculada a raiz quadrada de um numero
// a partir de um ERRO e de um PASSO.

// Inclusao de bibliotecas e definicao de constantes
#include <stdio.h>
#include <stdlib.h>
#define RADICANDO 27        // Raiz a ser calculada
#define ERRO 0.0000001      // Erro definido
#define PASSO 0.00001       // Passo definido

// Declaracao de funcoes
float calcula_raiz(float);

// Programa principal
int main(){
    // Chamada da funcao de calculo
    float num = calcula_raiz(RADICANDO);
    
    // Impressao do resultado
    printf("%.4f\n",num);

    return 0; 
}

// Funcao que realiza o calculo da raiz quadrada
float calcula_raiz(float num){
    // Declaracao da variavel de resultado
    float res = 0;

    // Aproximacao ate que esteja dentro do ERRO
    while(num - res*res > ERRO){
        res += PASSO;
    }

    // Retorna o resultado
    return res;
}