#ifndef AUXILIARES_H
#define AUXILIARES_H

typedef struct cpu CPU;
typedef struct ram RAM;

typedef struct
{
    int add1, add2, add3, opcode;

} Instrucao;

// Programas basicos mas de utilização pratica ATENÇÃO NA RAM
void Soma(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal);    // conferir modo de usar daniel
void Subtrai(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal); // conferir modo de usar daniel
int pegarResultado(RAM *ram, CPU *cpu, int endereco);
void colocarNaRam(RAM *ram, CPU *cpu, int endereco, int valor);

void imprimirMatriz(int linhas, int colunas, int matriz[linhas][colunas]);
int pegarMult(RAM *ram, CPU *cpu);
int pegarDiv(RAM *ram, CPU *cpu);

#endif
