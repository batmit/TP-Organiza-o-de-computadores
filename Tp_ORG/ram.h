#ifndef RAM_H
#define RAM_H
#include "hd.h"
#include <stdbool.h>

// RAM
#define TAM_RAM 32 // tamanho da RAM

// CACHE
#define TAM_L1 4
#define TAM_L2 8
#define TAM_L3 16

typedef struct ramvet RamVet;

typedef struct cacheLine CacheLine;

typedef struct ram RAM;




RAM *criarRAM(int tam);
RAM *criarRAM_vazia(int tam);
RAM *criarRAM_aleatoria(int tam);
void reinicializarRAM(RAM *r, int tam);
RAM *inicializarRAMdoHD(int tam);
void colocarNaRAM(RAM *r, int chave, int valor);
// Cache

void rebaixarParaL3(RAM *r, int endereco, int valor, long tempoOriginal);
void rebaixarParaL2(RAM *r, int endereco, int valor, long tempoOriginal);
void promoverParaL1(RAM *r, int endereco, int valor, int blocoTag);
void promoverParaL2(RAM *r, int endereco, int valor, int blocoTag);
void promoverParaL3(RAM *r, int endereco, int valor, int blocoTag);

void simularBuffer(RAM *r, CacheLine *Cache3, int id);
bool buscarNaRam(RAM *r, int endereco);
bool buscarNaL3(RAM *r, int endereco);
bool buscarNaL2(RAM *r, int endereco);
int buscarNaL1(RAM *r, int endereco);

void setDado(RAM *r, int endereco, int conteudo);
int getDado(RAM *r, int endereco);
void imprimirRAM(RAM *r);
void imprimirCaches(RAM *r);
void destroiRAM(RAM *r);

void blocoTag(int endereco, int *resto, int *bloco);

#endif