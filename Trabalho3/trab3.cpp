#include <iostream>
#include <stdio.h>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>
using namespace std;

#define TAM_MEM_P 100
#define TAM_MEM_S 20000
#define PAG_SIZE 100

class pagina
{
private:
public:
    char dados[PAG_SIZE];
    int process_id;
    int R;
    pagina();
    pagina(int id);
    ~pagina();
};

class processo
{
private:
    int tam;
public:
    int id;
    vector<pagina*> paginas;
    map<pagina*,pagina*> tabela_paginas;
    processo(int identificador, int tamanho);
    ~processo();
};

class lista_circular
{
private:
public:
    vector<pagina*> mem_p;
    int pont;
    lista_circular();
    ~lista_circular();
    void carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos,int flag);
};

class mem
{
private:
public:
    vector<pagina*> paginas_mem;
    int tam;
    mem(int tam);
    ~mem();
};

pagina::pagina(int id)
{
    process_id = id;
    R = 0;
    for(int i=0; i<PAG_SIZE; i++){
        dados[i] = 'a'+i;
    }
}

pagina::pagina()
{
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

lista_circular::lista_circular()
{
}

lista_circular::~lista_circular()
{
}

void lista_circular::carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos, int flag){
    pagina * aux = new pagina();
    int aux2;
    *aux = *pag;
    aux->R = 1;
    if(mem_p.size() < TAM_MEM_P/PAG_SIZE){
        cout << "   Memória principal não está cheia." << endl;
        mem_p.push_back(aux);
        proc->tabela_paginas[pag] = aux;
        cout << "   Página carregada na memória principal." << endl;
    }else{
        int i, j;
        map<pagina*,pagina*>::iterator it;
        cout << "   Memória principal cheia." << endl;
        if(flag){
            cout << "\n   Algorimo LRU em execução..." << endl;
        }else{
            cout << "\n   Algorimo do relógio em execução..." << endl;
        }
        
        while(true){
            if(mem_p[pont]->R == 0){
                for(i=0; i<processos.size(); i++){
                    if(processos[i]->id == mem_p[pont]->process_id){
                        for(it = processos[i]->tabela_paginas.begin(); it != processos[i]->tabela_paginas.end(); it++){
                            if(it->second == mem_p[pont]){
                                it->second = NULL;
                                break;
                            }
                        }
                        break;
                    }
                }
                mem_p[pont] = aux;
                proc->tabela_paginas[pag] = aux;
                
                for(j = 0; j < processos[i]->paginas.size(); j++){
                    if(processos[i]->paginas[j] == it->first){
                        break;
                    }
                }
                cout << "     > Página a ser substituída: página " << (j)*PAG_SIZE << "~" << (j+1)*PAG_SIZE -1 << " do Processo " << i+1 << "\n" << endl;

                break;
            }else{
                mem_p[pont]->R--;
                pont++;
                pont = pont % (TAM_MEM_P/PAG_SIZE);
            }
        }
        pont++;
        pont = pont % (TAM_MEM_P/PAG_SIZE);
    }
} 

processo * cria_processo(int id, int tam, mem & mem_s){
    processo * x = new processo(id,tam);
    int num_pag = ceil(tam/PAG_SIZE);
    if(num_pag <= (TAM_MEM_S/PAG_SIZE - mem_s.paginas_mem.size())){
        cout << "   Memória secundária não está cheia." << endl;
        for(int i=0; i< x->paginas.size(); i++){
            mem_s.paginas_mem.push_back(x->paginas[i]);
        }
        cout << "   Processo " << id << " criado e alocado na memória virtual com sucesso." << endl;
    }
    return x;
}

void * le(int iden, int end, vector<processo*> & processos, lista_circular & mem_p, int flag){
    processo * aux;
    pagina * aux2;
    pagina * dado;
    for(int i=0; i<processos.size(); i++){
        if(processos[i]->id == iden){
            aux = processos[i];
        }
    }
    int pag = floor(end/PAG_SIZE);
    int pos = end % PAG_SIZE;
    
    aux2 = aux->paginas[pag];
    if(aux->tabela_paginas[aux2] == NULL){
        mem_p.carrega_mem(aux,aux2,processos,1);
    }else{
        cout << "   Página já está na memória principal." << endl;
        if(flag == 1){
            aux->tabela_paginas[aux2]->R++;
        }
    }

    dado = aux->tabela_paginas[aux2];

    cout << "   Dado lido: " << dado->dados[pos] << endl;
    
}

void * escreve(int iden, int end, vector<processo*> & processos, lista_circular & mem_p, int flag){
    processo * aux;
    pagina * aux2;
    pagina * dado;
    for(int i=0; i<processos.size(); i++){
        if(processos[i]->id == iden){
            aux = processos[i];
        }
    }
    int pag = floor(end/PAG_SIZE);
    int pos = end % PAG_SIZE;
    
    aux2 = aux->paginas[pag];
    if(aux->tabela_paginas[aux2] == NULL){
        mem_p.carrega_mem(aux,aux2,processos,1);
    }else{
        cout << "   Página já está na memória principal." << endl;
        if(flag == 1){
            aux->tabela_paginas[aux2]->R++;
        }
    }

    dado = aux->tabela_paginas[aux2];

    dado->dados[pos] = '*';
    aux2->dados[pos] = '*';
    
    cout << "   Dado escrito: " << dado->dados[pos] << endl;
    cout << "   Atualização da memória secundária realizada." << endl;
}

mem::mem(int tam)
{
    mem::tam = tam;
}

mem::~mem()
{
}

int le_arq(FILE * arq, int& proc, char & inst, int & tam){
    char aux[5];

    if(fscanf(arq,"%s %c %d", aux, &inst, &tam) == EOF){
        return 1;
    }
    proc = atoi(aux+1);
    return 0;
}



int main(){
    cout << " --- Simulador de gerenciamento de memória virtual com paginação --- \n\n";
    lista_circular * mem_p = new lista_circular();
    mem mem_s(TAM_MEM_S);
    vector<processo*> tabela_processos;
    FILE * arq;
    arq = fopen("entrada.txt","rt");
    if(arq == NULL){
        cout << "Arquivo nao encontrado" << endl;
        return 0;
    }
    
    char inst;
    int tam, proc;
    while(!le_arq(arq,proc,inst,tam)){
        cout << "--> Processo: " << proc << " | " << " Instrucao: " << inst << " | " << " Endereço/Tamanho: " << tam << endl;
        switch (inst)
        {
        case 'C':
            tabela_processos.push_back(cria_processo(proc,tam,mem_s));
            getc(stdin);
            break;
        case 'R':
            le(proc,tam,tabela_processos,*mem_p,1);
            getc(stdin);
            break;
        case 'W':
            escreve(proc,tam,tabela_processos,*mem_p,1);
            getc(stdin);
            break;
        default:
            cout << "To fazendo nada" << endl;
            break;
        }
    }
    

    
}
