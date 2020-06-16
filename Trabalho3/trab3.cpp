#include <iostream>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>
using namespace std;

#define TAM_MEM_P 2000
#define TAM_MEM_S 20000
#define PAG_SIZE 100

class pagina
{
private:
    char dados[PAG_SIZE];
    int process_id;
public:
    int end;
    pagina(int id);
    ~pagina();
};

class processo
{
private:
    int id;
    int tam;
public:
    vector<pagina*> paginas;
    map<pagina*,pagina*> tabela_paginas;
    processo(int identificador, int tamanho);
    ~processo();
};

class mem
{
private:
public:
    vector<pagina*> paginas_mem;
    stack<int> prox_livre;
    mem(int tam);
    ~mem();
};

pagina::pagina(int id)
{
    process_id = id;
}

pagina::~pagina()
{
}

processo::processo(int identificador, int tamanho)
{
    id = identificador;
    tam = tamanho;
    int num_pags = ceil(tamanho/PAG_SIZE);
    for(int i=0; i<num_pags;i++){
        paginas.push_back(new pagina(identificador));
    }
}

processo::~processo()
{
}

processo * cria_processo(int id, int tam, mem & mem_s){
    processo * x = new processo(id,tam);
    int num_pag = ceil(tam/PAG_SIZE);
    if(num_pag <= (TAM_MEM_S/PAG_SIZE - mem_s.paginas_mem.size())){
        for(int i=0; i< x->paginas.size(); i++){
            x->paginas[i]->end = mem_s.prox_livre.top() * PAG_SIZE;
            mem_s.prox_livre.pop();
            mem_s.paginas_mem.push_back(x->paginas[i]);
        }
    }
}

mem::mem(int tam)
{
    for(int i=tam/PAG_SIZE; i>=0; i--){
        prox_livre.push(i);
    }
}

mem::~mem()
{
}



int main(){
    cout << " --- Simulador de gerenciamento de memória virtual com paginação --- \n\n";
    mem mem_p(TAM_MEM_P);
    mem mem_s(TAM_MEM_S);

    map<pagina*,pagina*> teste;
    
}
