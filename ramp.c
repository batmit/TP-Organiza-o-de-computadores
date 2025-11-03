#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"

RAM memoria;



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
    RAM *r = malloc(sizeof(RAM));
    r->mem = malloc(tam * sizeof(int));
    for(int i=0; i<tam; i++)
			r->mem[i] =rand(); // cria memoria com endereco aleatorio
    r->tamanho = tam;
    return r;

}
void setDado(int endereco, int conteudo);
void getDado(int endereco);
void imprimir(void);