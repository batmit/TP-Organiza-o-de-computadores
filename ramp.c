#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Instrucao.h"
#include "ram.h"

RAM memoria;

RAM* criarRAM(int tam){

    RAM *r = malloc(sizeof(RAM));
    r->mem = malloc(tam, sizeof(int)); 
    r->tamanho = tam;
    return r;
}


RAM* criarRAM_vazia(int tam);
RAM* criarRAM_aleatoria(int tam){

    RAM *r = malloc(sizeof(RAM));
    r->mem = calloc(tam, sizeof(int)); 
    r->tamanho = tam;
    return r;

}
void setDado(int endereco, int conteudo);
void getDado(int endereco);
void imprimir(void);