//Programa que exemplifica a utilização das system calls mmap, munmap e mprotect

//A system call mmap(void *addr , size_t len, int prot, int flags, int fd, off_t offset) aloca um espaço da memória virtual, posssuindo como argumentos: O endereço da memória que deverá ser alocada(se = NULL o sistema adota um esndereço qualquer);
//O tamanho em bytes da pagina á ser alocada; A proteção para tipos de acessos a memoria; As "flags" que determinam se as atualizações nessa memória serão visiveis a outros processos; O "file descriptor"; E o "offset"

//A system call mprotect(void *addr, size_t len, int prot) muda a proteção para tipos de acesso a memoria, possuindo como argumentos o endereço da memoria, o tamanho desta, e o novo tipo de proteção que ela possuirá

//A system call munmap(void *addr, size_t length) desaloca um espaço da memória virtaul, possuindo como argumentos o endereço da memória virtual a ser liberada e o da tamanho da memória

//system calls envolvidas(tempo em microssegundos):write(631);execve(479);mmap(125);mprotect(122);munmap(72);access(59);openat(48);fstat(47);brk(46);close(28);read(18);arch_prctl(14);

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

//o tamanho da pagina de endereçamento de um computador pode ser obtida através do comando "getconf PAGE_SIZE", a maioria dos computadores usam 4KB assim como a maquina virtual usada
#define PAGESIZE 4096

int main(){
	//mmap retornara um ponteiro com o endereço da memoria alocada ou com -1
	int *e = mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(e==(void *) -1){
		printf("Erro no mapeamento de memória \n");
	}else{
		printf("Endereço de memória virtual mapeado:%p \n", e);
	}

	//mprotect retornara 0 se alterar a proteção e -1 caso contrário
	int p = mprotect(e, PAGESIZE, PROT_READ);
	if(p==0){
		printf("Proteção da pagina mudado para apenas leitura \n");
	}
	else{
		printf("Não foi possível alterar a proteção da pagina \n");
	}

	//munmap retornara 0 se for possivel liberar a memoria e -1 caso contrário
	int s = munmap(e, PAGESIZE);
	if(s==0){
		printf("Mapeamento da pagina %p excluido \n", e);
	}
	else{
		printf("Não foi possível excluir o mapeamento da pagina %p \n", e);
	}
	return 0;
}
