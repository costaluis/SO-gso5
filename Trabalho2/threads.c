//João Pedro Borges de Castro - 10276720
//João Pedro Fidelis Belluzzo - 10716661
//Luis Fernando Costa de Oliveira - 10716532

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TAM_BUFFER 60           // Tamanho do buffer
#define QTD_PRODUTOR 1500       // Nro de vezes que o produtor tenta escrever no buffer
#define DELAY_PROD 10           // Delay entre cada producao de informacao pelo produtor
#define DELAY_CONS 50           // Delay entre cada consumo de informacao pelo consumidor

int contador=0;

void *produtor(void*);          // Funcao thread do produtor
void *consumidor(void*);        // Funcao thread do consumidor

typedef struct arg_info         // Buffer
{
    int * conteudo;            // Conteudo do buffer 
    int ** free_addr;          // Ponteiro para o proximo endereco livre
    int ** ocup_addr;          // Ponteiro para o ultimo endereco ocupado
    int *num_free_addr;        // Nro de enderecos livres
    int *num_ocup_addr;        // Nro de enderecos ocupados
}arg_info;

typedef struct thread_info      // Informacoes das threads
{
    pthread_t id;
    arg_info * arg;
}thread_info;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Objeto de aplicacao da Exclusao Mutua
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER; //Variável condicional

int main(){
    
    int iret1, iret2;            

    srand(time(NULL)); 
    
    // Criacao das threads
    thread_info *t1 = calloc(1,sizeof(thread_info)); 
    thread_info *t2 = calloc(1,sizeof(thread_info));

    arg_info * buffer = calloc(1,sizeof(arg_info));
    buffer->conteudo = (int*)calloc(TAM_BUFFER,sizeof(int));
    buffer->num_free_addr = (int*)calloc(1,sizeof(int));
    buffer->num_ocup_addr = (int*)calloc(1,sizeof(int));
    buffer->free_addr = (int**)calloc(1,sizeof(int*));
    buffer->ocup_addr = (int**)calloc(1,sizeof(int*));
    *(buffer->num_free_addr) = TAM_BUFFER;
    *(buffer->num_ocup_addr) = 0;
    *(buffer->free_addr) = buffer->conteudo;
    *(buffer->ocup_addr) = buffer->conteudo;
    
    t1->arg = buffer;
    t2->arg = buffer;
  
    // Inicializacao das threads
    iret1 = pthread_create(&(t1->id), NULL, produtor, (void*)t1);
    if(iret1){
        printf("Erro - Thread 1\n");
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create(&(t2->id), NULL, consumidor, (void*)t2);
    if(iret2){
        printf("Erro - Thread 1\n");
        exit(EXIT_FAILURE);
    }

    // Aguarda ate que as threads terminem
    pthread_join(t1->id,NULL);
    pthread_join(t2->id,NULL);

    // Libera espacos alocados e destroi o mutex
    free(t1);
    free(t2);
    free(buffer->conteudo);
    free(buffer->num_free_addr);
    free(buffer->num_ocup_addr);
    free(buffer->free_addr);
    free(buffer->ocup_addr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition_var);

    exit(EXIT_SUCCESS);
}

// Thread referente ao produtor
void * produtor(void * arg){
    thread_info *m = arg;

    while(contador < QTD_PRODUTOR){
        // A partir desse ponto, apenas essa thread manipula o buffer
        pthread_mutex_lock(&mutex);

        if(*m->arg->num_free_addr == 0 && *m->arg->num_ocup_addr == TAM_BUFFER){ //se estiver cheio
            printf("Buffer cheio, produtor em espera\n");
            pthread_cond_wait( &condition_var, &mutex ); //coloca o produtor em estado de espera
        }

        **(m->arg->free_addr) = 1950 + rand()%70; //calcula um número aleatório entre 1950 e 2020, que representa o ano de nascimento de uma pessoa

        if(*m->arg->num_free_addr != TAM_BUFFER && *m->arg->num_ocup_addr != 0){ //se não for o primeiro elemento do buffer
            (*m->arg->ocup_addr)++; // avança o ponteiro que indica o ultimo elemento acupado, em um
        }

        (*m->arg->num_free_addr)--;
        (*m->arg->num_ocup_addr)++;

        if(*m->arg->num_free_addr != 0 && *m->arg->num_ocup_addr != TAM_BUFFER){ //se não for o ultimo elemento do buffer
            (*m->arg->free_addr)++; //avança o ponteiro que indica o ultimo elemento livre, em um
        }
        //printa os elementos do Buffer após a execução do produtor
        int i;
        printf("Buffer após o produtor:");
        for(i=0;i<*m->arg->num_ocup_addr;i++){
            printf("%d ", m->arg->conteudo[i]);
        }
        printf("\n");
        // Libera o buffer para o consumidor
        pthread_mutex_unlock(&mutex);
        contador++;
        // Delay temporal entre as execucoes do loop
        usleep(DELAY_PROD);
    }
}

void * consumidor(void* arg){
    thread_info *m = arg;
    int aux;
    int c=0;
    while (contador < QTD_PRODUTOR || *m->arg->num_ocup_addr != 0){ //acaba somente quando o produtor executar todas as vezes e o buffer estiver vazio
        // A partir desse ponto, apenas essa thread manipula o buffer
        pthread_mutex_lock(&mutex);

        // Retira informacao do buffer e rearranja ponteiros e contadores
        if(*m->arg->num_ocup_addr){ //se não está vazio
            //captura um dado do buffer(ano de nascimento de uma pessoa) e calcula sua idade
            aux = **(m->arg->ocup_addr);
            printf("Idade:%d\n", 2020-aux);

            **(m->arg->ocup_addr) = '\0'; //limpa o elemento do buffer que foi utilizado
            
            if(*m->arg->num_free_addr != 0 && *m->arg->num_ocup_addr != TAM_BUFFER){ //se o buffer não estiver cheio
                (*m->arg->free_addr)--; //recua o ponteiro que indica o ultimo elemento livre, em um
            }
            else{
                c=1;
            }

            (*m->arg->num_free_addr)++;
            (*m->arg->num_ocup_addr)--;
            

            if(*m->arg->num_free_addr != TAM_BUFFER && *m->arg->num_ocup_addr != 0){ // se o buffer não ficar vazio
                (*m->arg->ocup_addr)--; //recua o ponteiro que indica o ultimo elemento ocupado, em um
            }
            //printa os elementos do Buffer após a execução do produtor
            int i;
            printf("Buffer após o consumidor:");
            for(i=0;i<*m->arg->num_ocup_addr;i++){
                printf("%d ", m->arg->conteudo[i]);
            }
            printf("\n");

            if(c){ //se o buffer estava cheio
                pthread_cond_signal( &condition_var ); // sinaliza o produtor à voltar a executar após a execução do consumidor
                c=0;
            }
        }
        // Libera o buffer para o produtor
        pthread_mutex_unlock(&mutex);
        // Delay temporal entre as execucoes do loop
        usleep(DELAY_CONS);
    }
}