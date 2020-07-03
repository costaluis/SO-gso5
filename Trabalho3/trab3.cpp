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
    pagina();                       // declaração de um construtor para objetos da classe pagina (usado quando não é necessário atribuir valores logo na construção do objeto)
    pagina(int id);                 // declaração de um construtor parametrizado para objetos da classe pagina (atribui valores logo na construção do objeto)
    ~pagina();                      // declaração do destrutor para objetos da classe pagina
};

// Classe relativa a um processo
class processo
{
private:
public:
    int tam;                                    // Tamanho em bytes do processo
    int id;                                     // ID do processo
    vector<pagina*> paginas;                    // Vetor de paginas referente ao processo
    map<pagina*,pagina*> tabela_paginas;        // Tabela de paginas referente ao processo
    processo(int identificador, int tamanho);   // Declaração do construtor parametrizado
    ~processo();                                // Declaração de destrutor
};

// Lista circular de paginas da memoria primaria
class lista_circular
{
private:
public:
    vector<pagina*> mem_p;          // Memoria primaria
    int pont;                       // Ponteiro que guarda o ID da proxima pagina
    lista_circular();               // Declaração do construtor
    ~lista_circular();              // Declaração do destrutor
    void carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos,int flag);    // função que carrega uma pagina na memória primaria 
};

// Classe referente a memoria
class mem
{
private:
public:
    vector<pagina*> paginas_mem;    // Vetor de paginas da memoria
    int tam;                        // Tamanho em bytes da memoria
    mem(int tam);                   // Declaração de construtor parametrizado
    ~mem();                         // Declaração de destrutor
};

// Construtor parametrizado da classe pagina (atribui valores na construção)
pagina::pagina(int id)
{
    process_id = id;                    //id do processo a qual a pagina pertence
    R = 0;                              //contador de uso pra o LRU
    for(int i=0; i<PAG_SIZE; i++){      //preenchimeto do conteudo da pagina
        dados[i] = 'a'+i;
    }
}

// Construtor da classe pagina
pagina::pagina()
{
}

// Destrutor da classe página
pagina::~pagina()
{
}

// Construtor parametrizado  da classe processo (atribui valores na construção)
processo::processo(int identificador, int tamanho)
{
    id = identificador;                                     //numero identificador do processo
    tam = tamanho;                                          //tamanho do processo
    int num_pags = ceil(tamanho/PAG_SIZE);                  //número de paginas que o processo possuirá
    for(int i=0; i<num_pags;i++){                           //alocação das paginas do processo
        paginas.push_back(new pagina(identificador));
    }
}

// Destrutor da classe processo
processo::~processo()
{
    for(int i=0; i<paginas.size();i++){
        delete paginas[i];
    }
}

// Construtor da classe lista circular
lista_circular::lista_circular()
{
}

// Destrutor da classe lista circular
lista_circular::~lista_circular()
{
    for(int i=0; i<mem_p.size();i++){
        delete(mem_p[i]);
    }
}

// Construtor parametrizado da classe mem (atribui, na construção, um valor ao tamanho da memória)
mem::mem(int tam)
{
    mem::tam = tam;
}

// Destrutor da classe mem
mem::~mem()
{
}

// Funcao da lista_circular que carrega uma pagina na memoria primaria
void lista_circular::carrega_mem(processo * proc, pagina * pag, vector<processo*> & processos, int flag){
    pagina * aux = new pagina();
    int aux2;
    *aux = *pag;
    aux->R = 1;
    if(mem_p.size() < TAM_MEM_P/PAG_SIZE){                              // Verifica se a memoria primaria esta cheia
        cout << "   Memória principal não está cheia." << endl;         // Se nao estiver, carrega a pagina na memoria primaria
        mem_p.push_back(aux);
        proc->tabela_paginas[pag] = aux;                                // Coloca a nova pagina na tabela de paginas do processo
        cout << "   Página carregada na memória principal." << endl;
    }else{                                                              // Se estiver cheia, um dos algoritmos (indicado por flag) sera utilizado para substituir uma pagina
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
            }else{                      // A pagina analisada nao eh a que sera substituida
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
    for(int i=0; i<processos.size(); i++){                  // Encontra o processo
        if(processos[i]->paginas.size()*100 >= end){        // Verifica o espaco de enderecamento do processo
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

// Função para realizar escrita
void escreve(int iden, int end, vector<processo*> & processos, lista_circular & mem_p, int flag,vector<int> & proc_susp){
    processo * aux;
    pagina * aux2;
    pagina * dado;
    if(!transf_ver(processos,end,iden,proc_susp)){  //verifica se o endereço pertence ao processo e se este não está suspenso
        return;                                     //caso algumas dessas condições não seja satisfeita, a função finaliza
    }
    for(int i=0; i<processos.size(); i++){          //percorre o vetor de processos e encontra o processo que possui a id "iden"
        if(processos[i]->id == iden){               //este processo será passado por referencia à variável aux
            aux = processos[i];
        }
    }
    int pag = floor(end/PAG_SIZE);                  //identifica qual pagina acessar
    int pos = end % PAG_SIZE;                       //identifica qual a posição à acessar na pagina
    
    aux2 = aux->paginas[pag];                       //a pagina é passada por referencia à variável aux2
    if(aux->tabela_paginas[aux2] == NULL){          //se a pagina não está na memória principal, ela é carregada para esta
        mem_p.carrega_mem(aux,aux2,processos,1);
    }else{                                          //caso a pagina já esta em memória principal
        cout << "   Página já está na memória principal." << endl;
        if(flag == 1){                              //se está sendo usado o LRU o contador de uso da pagina é incrementado
            aux->tabela_paginas[aux2]->R++;
        }
    }

    dado = aux->tabela_paginas[aux2];

    dado->dados[pos] = '*';                         //escreve na memória primaria
    aux2->dados[pos] = '*';                         //escreve na memória secundaria
    
    cout << "   Dado escrito: " << dado->dados[pos] << endl;
    cout << "   Atualização da memória secundária realizada." << endl;
}

// Função para instrução a ser executada pela CPU
void inst_CPU(vector<int> proc_susp, vector<processo*> processos, int id, int inst){
    bool flag = false;
    for(int i=0; i<proc_susp.size(); i++){                              // percorre o vetor de processos suspensos, verificando, através da id, se o processo que deve realizar a instrução está entre eles
        if(proc_susp[i]==id){                                           // caso esteja, a função informa que o processo está suspenso e termina sua execução
            cout << "   Processo " << id << " esta suspenso." << endl;
            return;
        }
    }
    for(int i=0; i<processos.size(); i++){                              // percorre o vetor de processos criados, verificando, através da id, se o processo que deve realizar a instrução está entre eles
        if(processos[i]->id == id){                                     // caso esteja, sinaliza uma flag
            flag = true;
        }
    }
    if(!flag){                                                          // caso a flag não tenha sido sinalizada a função informa que o processo não existe e termina sua execução
        cout << "   Processo " << id << " nao existe." << endl;
        return;
    }
    cout << "   Processo " << id << " executou a instrucao " << inst << " pela CPU." << endl;       //caso a função tenha chegado a esse ponto (significando que o processo está no vetor de processos criados) a função imprie que ele realizou a execução pela CPU
}

// Função para executar instrução de entrada e saída
void inst_IO(vector<int> proc_susp, vector<processo*> processos, int id, int inst){
    bool flag = false;
    for(int i=0; i<proc_susp.size(); i++){                              // percorre o vetor de processos suspensos, verificando, através da id, se o processo que deve realizar a instrução está entre eles
        if(proc_susp[i]==id){                                           // caso esteja, a função informa que o processo está suspenso e termina sua execução
            cout << "   Processo " << id << " esta suspenso." << endl;
            return;
        }
    }
    for(int i=0; i<processos.size(); i++){                              // percorre o vetor de processos criados, verificando, através da id, se o processo que deve realizar a instrução está entre eles
        if(processos[i]->id == id){                                     // caso esteja, sinaliza uma flag
            flag = true;
        }
    }
    if(!flag){                                                          // caso a flag não tenha sido sinalizada a função informa que o processo não existe e termina sua execução
        cout << "   Processo " << id << " nao existe." << endl;
        return;
    }
    cout << "   Processo " << id << " executou a instrucao " << inst << " de IO." << endl;      //caso a função tenha chegado a esse ponto (significando que o processo está no vetor de processos criados) a função imprie que ele realizou a instrução de IO
}

// Função para leitura do arquivo de entrada
int le_arq(FILE * arq, int& proc, char & inst, int & tam){
    char aux[5];                                            // string armazenará o nome do processo, exemplo: P7

    if(fscanf(arq,"%s %c %d", aux, &inst, &tam) == EOF){    // lê uma linha do arquivo, caso seja o final do arquivo, retorna 1
        return 1;
    }
    proc = atoi(aux+1);                                     // carregará o numero do processo, em int, na variavel proc 
    return 0;
}

int main(){
    cout << " --- Simulador de gerenciamento de memória virtual com paginação --- \n\n";
    lista_circular * mem_p = new lista_circular();          //cria um objeto mem_p (memória principal) do tipo lista_circular
    mem mem_s(TAM_MEM_S);                                   //cria um objeto mem_s (memória secundária) do tipo mem
    vector<processo*> tabela_processos;                     //declara o vetor tabela_processos que armazenará ponteiros para os endereços de memória virtual dos processos criados
    vector<int> processos_suspensos;                        //declara o vetor de inteiros "processos_suspensos", que armazenará o id dos processos suspensos 
    FILE * arq;                                             // O arquivo de texto "entrada.txt" é aberto para leitura
    arq = fopen("entrada.txt","rt");
    if(arq == NULL){                                        //Se não existir o arquivo, é impresso na tela "Arquivo não encontrado"
        cout << "Arquivo nao encontrado" << endl;
        return 0;
    }
    char inst;                                              // "inst" armazenará a instrução do processo
    int tam, proc;                                          // "proc" armazenará o número identificador do processo e "tam" o tamanho ou o endereço de memória que o processo deverá utilizar
    while(!le_arq(arq,proc,inst,tam)){                      //executará enquanto for 0, ou seja, até achar o final do arquivo
        cout << "--> Processo: " << proc << " | " << " Instrucao: " << inst << " | " << " Endereço/Tamanho: " << tam << endl;           // imprime a as instruções que serão executadas
        switch (inst)
        {
        case 'C':           // caso a instrução seja de criação do processo
            tabela_processos.push_back(cria_processo(proc,tam,mem_s,processos_suspensos));
            getc(stdin);
            break;
        case 'R':           // caso a instrução seja de leitura
            le(proc,tam,tabela_processos,*mem_p,1,processos_suspensos);
            getc(stdin);
            break;
        case 'W':           // caso a instrução seja de escrita
            escreve(proc,tam,tabela_processos,*mem_p,1,processos_suspensos);
            getc(stdin);
            break;
        case 'I':           // caso a instrução seja de IO
            inst_IO(processos_suspensos,tabela_processos,proc,tam);
            getc(stdin);
            break;
        case 'P':           // caso a instrução seja execução pela CPU
            inst_CPU(processos_suspensos,tabela_processos,proc,tam);
            getc(stdin);
            break;
        default:            // caso a instrução seja diferente das definidas
            cout << "Instrucao invalida!" << endl;
            getc(stdin);
            break;
        }
    }
    delete(mem_p);
    for(int i=0; i<tabela_processos.size();i++){
        delete tabela_processos[i];
    }
    fclose(arq);
    return 0;
        
}
