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
			r->mem[i] =rand(); // cria memoria com endereco aleatorio
    return r;

}

void setDado(RAM *r,int endereco, int conteudo){
    r->mem[endereco] = conteudo;
}

int getDado(RAM *r,int endereco){
    return r->mem[endereco] ;
}

void imprimirRAM(RAM *r){
    printf("Conteudo da RAM\n");
    for(int i=0; i<r->tamanho; i++){
			printf("%d ",r->mem[i]);
            if(i < r->tamanho - 1)
                printf(", ");
		
    }
    printf("\n");

}