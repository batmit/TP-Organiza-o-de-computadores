#ifndef RAM_H
#define RAM_H

#include <stdbool.h>
// RAM
#define TAM_RAM 50 // tamanho da RAM

// CACHE
#define TAM_L1 4
#define TAM_L2 8
#define TAM_L3 16

typedef struct ram RAM;

typedef struct
{
    int tag;           // endereço original da RAM
    int dado;          // valor guardado
    int valido;        // 0 = vazio, 1 = ocupado
    long ultimoAcesso; // contador que armazena o "tempo" do ultimo uso

} CacheLine;

RAM *criarRAM(int tam);
RAM *criarRAM_vazia(int tam);
RAM *criarRAM_aleatoria(int tam);
void reinicializarRAM(RAM *r, int tam);

// Cache

void rebaixarParaL3(RAM *r, int endereco, int valor, long tempoOriginal);
void rebaixarParaL2(RAM *r, int endereco, int valor, long tempoOriginal);
void promoverParaL1(RAM *r, int endereco, int valor);
void promoverParaL2(RAM *r, int endereco, int valor);
void promoverParaL3(RAM *r, int endereco, int valor);

void simularBuffer(RAM *r, CacheLine *Cache3, int id);
void buscarNaRam(RAM *r, int endereco);
void buscarNaL3(RAM *r, int endereco);
void buscarNaL2(RAM *r, int endereco);
int buscarNaL1(RAM *r, int endereco);

void setDado(RAM *r, int endereco, int conteudo);
int getDado(RAM *r, int endereco);
void imprimirRAM(RAM *r);
void imprimirCaches(RAM *r);
void destroiRAM(RAM *r);

#endif