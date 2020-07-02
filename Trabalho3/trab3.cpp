// Inclusao de bibliotecas
#include <iostream>
#include <stdio.h>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>
using namespace std;

// Definicoes de constantes
#define TAM_MEM_P 100               // Tamanho em bytes da memoria primaria
#define TAM_MEM_S 20000             // Tamanho em bytes da memoria secundaria
#define PAG_SIZE 100                // Tamanho em bytes da pagina (e moldura)


// Classe relativa a uma pagina
class pagina
{
private:
public:
    char dados[PAG_SIZE];           // Conteudo da pagina
    int process_id;                 // ID do processo referente a pagina
    int R;                          // bit de controle para algoritmos de substituicao de páginas (LRU e relogio)
    pagina();
    pagina(int id);
    ~pagina();
};

// Classe relativa a um processo
class processo
{
private:
public:
    int tam;                                // Tamanho em bytes do processo
    int id;                                 // ID do processo
    vector<pagina*> paginas;                // Vetor de paginas referente ao processo
    map<pagina*,pagina*> tabela_paginas;    // Tabela de paginas referente ao processo
    processo(int identificador, int tamanho);
    ~processo();
};

// Lista circular de paginas da memoria primaria
class lista_circular
{
private:
public:
    vector<pagina*> mem_p;          // Memoria primaria
    int pont;                       // Ponteiro que guarda o ID da proxima pagina
    lista_circular();
    ~lista_circular();
    void carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos,int flag);
};

// Classe referente a memoria
class mem
{
private:
public:
    vector<pagina*> paginas_mem;    // Vetor de paginas da memoria
    int tam;                        // Tamanho em bytes da memoria
    mem(int tam);
    ~mem();
};

// Construtor da classe pagina
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

// Construtor da classe processo
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

// Funcao da lista_circular que carrega uma pagina na memoria primaria
void lista_circular::carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos, int flag){
    pagina * aux = new pagina();
    int aux2;
    *aux = *pag;
    aux->R = 1;
    if(mem_p.size() < TAM_MEM_P/PAG_SIZE){     // Verifica se a memoria primaria esta cheia
        cout << "   Memória principal não está cheia." << endl;     // Se nao estiver, carrega a pagina na memoria primaria
        mem_p.push_back(aux);
        proc->tabela_paginas[pag] = aux;
        cout << "   Página carregada na memória principal." << endl;
    }else{      // Se estiver cheia, um dos algoritmos (indicado por flag) sera utilizado para substituir uma pagina
        int i, j;
        map<pagina*,pagina*>::iterator it;
        cout << "   Memória principal cheia." << endl;
        if(flag){
            cout << "\n   Algorimo LRU em execução..." << endl;
        }else{
            cout << "\n   Algorimo do relógio em execução..." << endl;
        }
        
        // Encontra a pagina a ser substituida
        while(true){
            if(mem_p[pont]->R == 0){        // Encontrou a pagina a ser substituida
                for(i=0; i<processos.size(); i++){      // Encontra o processo relativo a essa pagina
                    if(processos[i]->id == mem_p[pont]->process_id){
                        for(it = processos[i]->tabela_paginas.begin(); it != processos[i]->tabela_paginas.end(); it++){
                            if(it->second == mem_p[pont]){      // Encontra a pagina na tabela de paginas do processo
                                it->second = NULL;              // e atualiza o endereco na memoria fisica,
                                break;                          // indicando que a pagina nao esta mais na mem_p
                            }
                        }
                        break;
                    }
                }           
                mem_p[pont] = aux;
                proc->tabela_paginas[pag] = aux;                // Atualiza a tabela de paginas do processo
                
                // Loop para auxiliar o print das informacoes
                for(j = 0; j < processos[i]->paginas.size(); j++){
                    if(processos[i]->paginas[j] == it->first){
                        break;
                    }
                }
                cout << "     > Página a ser substituída: página " << (j)*PAG_SIZE << "~" << (j+1)*PAG_SIZE -1 << " do Processo " << i+1 << "\n" << endl;

                break;
            }else{              // A pagina analisada nao eh a que sera substituida
                mem_p[pont]->R--;       // Decrementa o bit de controle do LRU/relogio
                pont++;                 // Vai para a proxima pagina
                pont = pont % (TAM_MEM_P/PAG_SIZE);     // Volta para o inicio da lista quando exceder o tamanho da memoria (lista circular)
            }
        }
        pont++;
        pont = pont % (TAM_MEM_P/PAG_SIZE);
    }
} 

// Funcao referente a criacao de um processo
processo * cria_processo(int id, int tam, mem & mem_s, vector<int> & proc_susp){
    processo * x = new processo(id,tam);
    int num_pag = ceil(tam/PAG_SIZE);           // Numero de paginas do processo
    if(num_pag <= (TAM_MEM_S/PAG_SIZE - mem_s.paginas_mem.size())){     // Se couber na memoria secundaria...
        cout << "   Memória secundária não está cheia." << endl;
        for(int i=0; i< x->paginas.size(); i++){            // Aloca as paginas do processo
            mem_s.paginas_mem.push_back(x->paginas[i]);
        }
        cout << "   Processo " << id << " criado e alocado na memória virtual com sucesso." << endl;
    }else{          // Se nao couber, suspende o processo
        proc_susp.push_back(id);
        cout << "   Memoria secundaria cheia." << endl << "   Processo suspenso." << endl;
    }
    return x;
}

// Funcao que faz verificacoes sobre um processo
bool transf_ver(vector<processo*> processos, int & end, int proc, vector<int> & proc_susp){
    for(int i=0; i<processos.size(); i++){      // Encontra o processo
        if(processos[i]->paginas.size()*100 >= end){    // Verifica o espaco de enderecamento do processo
            if(processos[i]->id == proc){
                for(int j=0; j<proc_susp.size(); j++){      // Verifica se o processo esta suspenso
                    if(proc_susp[j]==proc){
                        cout << "   Processo " << proc << " esta suspenso." << endl;
                        return false;
                    }
                }
                return true;
            }else{                              // Endereco em questao nao pertence ao processo
                cout << "   Endereco nao pertence ao processo!" << endl;    
                proc_susp.push_back(proc);      // Suspende o processo
                cout << "   Processo " << proc << " suspenso." << endl;
                return false;
            }
        }else{
            end -= processos[i]->paginas.size()*100;
        }
    }
    cout << "   Endereco nao pertence ao processo!" << endl;    // Endereco em questao nao pertence ao processo 
    proc_susp.push_back(proc);                                  // Suspende o processo
    cout << "   Processo " << proc << " suspenso." << endl;
    return false;
}


// Funcao relativa a operacao de leitura
void le(int iden, int end, vector<processo*> & processos, lista_circular & mem_p, int flag, vector<int> & proc_susp){
    processo * aux;
    pagina * aux2;
    pagina * dado;
    if(!transf_ver(processos,end,iden,proc_susp)){  // Faz verificacoes quanto ao endereco e ao processo
        return;
    }
    for(int i=0; i<processos.size(); i++){  // Encontra o processo relativo ao ID passado
        if(processos[i]->id == iden){
            aux = processos[i];
        }
    }
    int pag = floor(end/PAG_SIZE);      // Calcula a pagina desejada
    int pos = end % PAG_SIZE;           // Calcula o endereco dentro da pagina desejada
    
    aux2 = aux->paginas[pag];
    // Faz o mapeamento das memorias
    if(aux->tabela_paginas[aux2] == NULL){      // Se a pagina nao estiver na memoria primaria...
        mem_p.carrega_mem(aux,aux2,processos,1);        // Chama a funcao de carregar a pagina na memoria
    }else{                                       // Se a pagina estiver na memoria primaria
        cout << "   Página já está na memória principal." << endl;
        if(flag == 1){      // Se o algorimo de subst. de paginas for o LRU...
            aux->tabela_paginas[aux2]->R++;        // Incrementa o bit de "pagina referenciada"
        }
    }

    dado = aux->tabela_paginas[aux2];

    cout << "   Dado lido: " << dado->dados[pos] << endl;
    
}

void escreve(int iden, int end, vector<processo*> & processos, lista_circular & mem_p, int flag,vector<int> & proc_susp){
    processo * aux;
    pagina * aux2;
    pagina * dado;
    if(!transf_ver(processos,end,iden,proc_susp)){
        return;
    }
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

void inst_CPU(vector<int> proc_susp, vector<processo*> processos, int id, int inst){
    bool flag = false;
    for(int i=0; i<proc_susp.size(); i++){
        if(proc_susp[i]==id){
            cout << "   Processo " << id << " esta suspenso." << endl;
            return;
        }
    }
    for(int i=0; i<processos.size(); i++){
        if(processos[i]->id == id){
            flag = true;
        }
    }
    if(!flag){
        cout << "   Processo " << id << " nao existe." << endl;
        return;
    }

    cout << "   Processo " << id << " executou a instrucao " << inst << " pela CPU." << endl;
}

void inst_IO(vector<int> proc_susp, vector<processo*> processos, int id, int inst){
    bool flag = false;
    for(int i=0; i<proc_susp.size(); i++){
        if(proc_susp[i]==id){
            cout << "   Processo " << id << " esta suspenso." << endl;
            return;
        }
    }
    for(int i=0; i<processos.size(); i++){
        if(processos[i]->id == id){
            flag = true;
        }
    }
    if(!flag){
        cout << "   Processo " << id << " nao existe." << endl;
        return;
    }
    cout << "   Processo " << id << " executou a instrucao " << inst << " de IO." << endl;
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
    vector<int> processos_suspensos;
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
            tabela_processos.push_back(cria_processo(proc,tam,mem_s,processos_suspensos));
            getc(stdin);
            break;
        case 'R':
            le(proc,tam,tabela_processos,*mem_p,1,processos_suspensos);
            getc(stdin);
            break;
        case 'W':
            escreve(proc,tam,tabela_processos,*mem_p,1,processos_suspensos);
            getc(stdin);
            break;
        case 'I':
            inst_IO(processos_suspensos,tabela_processos,proc,tam);
            break;
        case 'P':
            inst_CPU(processos_suspensos,tabela_processos,proc,tam);
            break;
        default:
            cout << "Instrucao invalida!" << endl;
            break;
        }
    }
    

    
}
