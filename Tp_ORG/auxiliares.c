#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "cpu.h"
#include "programas.h"
#include "auxiliares.h"

#define TAM_RAM 50

/* OPCODE
    -1 = Halt
    0 soma
    1 subtrai
    2 copia do registrador para RAM
    3 copia da RAM para o registrador
    4 salva conteudo externo no registrador
    5 obtem conteudo externo do registrador
    */

void Soma(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal)
{

    Instrucao *trecho_soma = (Instrucao *)malloc(2 * sizeof(Instrucao));

    trecho_soma[0].opcode = 0;      // Soma
    trecho_soma[0].add1 = pos1;     // Pega da RAM[end1]
    trecho_soma[0].add2 = pos2;     // Pega da RAM[end2]
    trecho_soma[0].add3 = posFinal; // Salva na RAM[endDest]

    trecho_soma[1].opcode = -1; // Halt

    setPrograma(cpu, trecho_soma);
    iniciar(ram, cpu);
}

void Subtrai(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal)
{

    Instrucao *trecho_soma = (Instrucao *)malloc(2 * sizeof(Instrucao));

    trecho_soma[0].opcode = 1;      // Subtração
    trecho_soma[0].add1 = pos1;     // Pega da RAM[end1]
    trecho_soma[0].add2 = pos2;     // Pega da RAM[end2]
    trecho_soma[0].add3 = posFinal; // Salva na RAM[endDest]

    trecho_soma[1].opcode = -1; // Halt

    setPrograma(cpu, trecho_soma);
    iniciar(ram, cpu);
}

void imprimirMatriz(int linhas, int colunas, int matriz[linhas][colunas])
{
    printf("--- Imprimindo Matriz (%dx%d) ---\n", linhas, colunas);
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            // Imprime o número e um 'tab' para alinhamento
            printf("%d\t", matriz[i][j]);
        }
        // Pula uma linha no final de cada linha da matriz
        printf("\n");
    }
    printf("----------------------------------\n");
}

void inverterString(char *str)
{
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++)
    {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int pegarMult(RAM *ram, CPU *cpu)
{
    // O resultado da multiplicação fica na RAM[0]
    return pegarResultado(ram, cpu, 0);
}

int pegarDiv(RAM *ram, CPU *cpu)
{
    // O resultado da divisao fica na RAM[3]
    return pegarResultado(ram, cpu, 3);
}

int pegarResultado(RAM *ram, CPU *cpu, int endereco)
{

    Instrucao *trecho1 = (Instrucao *)malloc(3 * sizeof(Instrucao));

    trecho1[0].opcode = 3;      // opcode: RAM -> Registrador
    trecho1[0].add1 = 1;        // registrador1
    trecho1[0].add2 = endereco; // Posição da RAM a ser lida

    trecho1[1].opcode = 5; // opcode: Registrador -> Instrução
    trecho1[1].add1 = 1;   // registrador1
    trecho1[1].add2 = -1;  // Espaço reservado para o resultado
    trecho1[1].add3 = -1;

    trecho1[2].opcode = -1;

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);

    return trecho1[1].add2;
}

void colocarNaRam(RAM *ram, CPU *cpu, int endereco, int valor)
{
    Instrucao *trecho1 = (Instrucao *)malloc(3 * sizeof(Instrucao));

    // reg[1] = valor
    trecho1[0].opcode = 4;
    trecho1[0].add1 = 1;     // registrador1
    trecho1[0].add2 = valor; // O valor a ser carregado

    // RAM[endereco] = reg[1]
    trecho1[1].opcode = 2;
    trecho1[1].add1 = 1;        // registrador1
    trecho1[1].add2 = endereco; // Posicao da RAM

    trecho1[2].opcode = -1;

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);
}

void multPosicoesRAM(RAM *ram, CPU *cpu, int pos1, int pos2, int posFinal)
{
    // Zera a posição final
    colocarNaRam(ram, cpu, posFinal, 0);

    // Pega o contador do loop da RAM[2]
    int multiplicador = pegarResultado(ram, cpu, pos2);

    // Loop 'multiplicador' times
    for (int i = 0; i < multiplicador; i++)
    {
        // RAM[posFinal] = RAM[posFinal] + RAM[1]
        Soma(ram, cpu, posFinal, pos1, posFinal);
    }
}

void programaMultSemPrint(RAM *ram, CPU *cpu, int multiplicando, int multiplicador)
{

    /*  RAM[0]: resultado
        RAM[1]: multiplicando */

    reinicializarRAM(ram, 2);

    /// salva o multiplicando na RAM[1]
    colocarNaRam(ram, cpu, 1, multiplicando);

    for (int i = 0; i < multiplicador; i++) // repete "multiplicador" vezes
    {
        Soma(ram, cpu, 0, 1, 0);
    }

    //printf("O resultado da multiplicacao e: %d\n", pegarMult(ram, cpu));
}