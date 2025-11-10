#ifndef PROGRAMAS_H
#define PROGRAMAS_H

void programaAleatorio(RAM *ram, CPU *cpu, int qdeIntrucoes);
void programaFat(RAM *ram, CPU *cpu, int fat);
void programaMult(RAM *ram, CPU *cpu, int multiplicando, int multiplicador);
void programaDiv(RAM *ram, CPU *cpu, int dividendo, int divisor);
void programaSomaMatriz(RAM *ram, CPU *cpu, int cardinalidade);
void programaRaizQuadrada(RAM *ram, CPU *cpu, int numero);
void programaRaizCubica(RAM *ram, CPU *cpu, int numero);
void programaRaioEsfera(RAM *ram, CPU *cpu, int volume);
void programaBin_Dec(RAM *ram, CPU *cpu, char *binario);
void programaBin_Dec(RAM *ram, CPU *cpu, char *binario);

void imprimirMatriz(int linhas, int colunas, int matriz[linhas][colunas]);

#endif