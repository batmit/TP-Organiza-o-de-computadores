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

void programaMultSemPrint(RAM *ram, CPU *cpu, int multiplicando, int multiplicador);

void imprimirMatriz(int linhas, int colunas, int matriz[linhas][colunas]);
void inverterString(char *str);

int pegarMult(RAM *ram, CPU *cpu);
int pegarDiv(RAM *ram, CPU *cpu);

void multPosicoesRAM(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal);

char* HexParaBin(char hex);
int maiusucla(int c);

#endif
