#ifndef RAM_H
#define RAM_H

#define TAM 64

typedef struct ram RAM;



RAM* criarRAM(int tam);
RAM* criarRAM_vazia(int tam);
RAM* criarRAM_aleatoria(int tam);

void setDado(RAM *r,int endereco, int conteudo);
int getDado(RAM *r,int endereco);

void imprimirRAM(RAM *r);
void destroiRAM(RAM *r);



#endif