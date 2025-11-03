#ifndef RAM_H
#define RAM_H

#define TAM 64

typedef struct{

    int *mem;
    int tamanho;
} RAM;



RAM* criarRAM(int tam);
RAM* criarRAM_vazia(int tam);
RAM* criarRAM_aleatoria(int tam);
void setDado(int endereco, int conteudo);
void getDado(int endereco);
void imprimir(RAM *r);



#endif