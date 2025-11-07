#ifndef PROGRAMAS_H
#define PROGRAMAS_H

void programaAleatorio(RAM *ram, CPU *cpu, int qdeIntrucoes);

void programaFat(RAM *ram, CPU *cpu, int fat);

void programaMult(RAM *ram, CPU *cpu, int multiplicando, int multiplicador);
void programaDiv(RAM *ram, CPU *cpu, int dividendo, int divisor);
void programaSomaMatriz(RAM *ram, CPU *cpu, int cardinalidade);
void imprimirMatriz(int rows, int cols, int matrix[rows][cols]);

#endif