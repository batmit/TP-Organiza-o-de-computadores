#ifndef AUXILIARES_H
#define AUXILIARES_H

// Programas basicos mas de utilização pratica ATENÇÃO NA RAM
void Soma(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal); // conferir modo de usar daniel
void Subtrai(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal); // conferir modo de usar daniel

void imprimirMatriz(int linhas, int colunas, int matriz[linhas][colunas]);
int pegarMult(RAM *ram, CPU *cpu);
int pegarDiv(RAM *ram, CPU *cpu);


#endif
