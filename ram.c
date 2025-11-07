#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"

struct ram { 
    int *mem;
    int tamanho;
};

RAM* criarRAM(int tam){

    RAM *r = malloc(sizeof(RAM));
    r->mem = malloc(tam * sizeof(int)); // cria memoria com lixo
    r->tamanho = tam;
    return r;

}

void reinicializarRAM(RAM *r, int tam){
    if (r->mem != NULL) {
        free(r->mem);
    }
    
    r->mem = calloc(tam, sizeof(int)); 
    if (r->mem == NULL) {
        printf("ERRO FATAL: Falha ao alocar %d ints para a RAM.\n", tam);
        exit(1);
    }
    r->tamanho = tam;
}


RAM* criarRAM_vazia(int tam){

    RAM *r = malloc(sizeof(RAM));
    r->mem = calloc(tam, sizeof(int));  // cria memoria com 0 (vazia)
    r->tamanho = tam;
    return r;

}

RAM* criarRAM_aleatoria(int tam){
    srand(time(NULL));
    RAM *r = criarRAM(tam);
    for(int i=0; i<tam; i++)
			r->mem[i] =rand() % 10; // cria memoria com endereco aleatorio
    return r;

}

void setDado(RAM *r,int endereco, int conteudo){
    r->mem[endereco] = conteudo; // aqui voce manda o endereço, a ram e o conteudo , ai ela guarda o conteudo naquela posição da ram
}

int getDado(RAM *r,int endereco){
    return r->mem[endereco] ; // voce manda o endereco e a ram, e ele te retorna o que esta armazenado
}

void imprimirRAM(RAM *r){
    printf("Conteudo da RAM\n"); // imrpime o conteudo da ram
    for(int i=0; i<r->tamanho; i++){
			printf("%d ",r->mem[i]);
            if(i < r->tamanho - 1)
                printf(", ");
		
    }
    printf("\n");

}

void destroiRAM(RAM *r){ // desalloca a memoria da ram
    free(r->mem);
    free(r);
}