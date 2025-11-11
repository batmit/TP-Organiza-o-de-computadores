#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Instrucao.h"
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

int pegarMult(RAM *ram, CPU *cpu)
{
    // O resultado da multiplicação fica na RAM[0]
    Instrucao* trecho1 = (Instrucao*) malloc(3 * sizeof(Instrucao));
    
    trecho1[0].opcode = 3; // RAM[0] -> reg1
    trecho1[0].add1 = 1;
    trecho1[0].add2 = 0;

    trecho1[1].opcode = 5; // reg1 -> instrução
    trecho1[1].add1 = 1;
    trecho1[1].add2 = -1;
    trecho1[2].opcode = -1;
    
    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);
    
    return trecho1[1].add2;
}


int pegarDiv(RAM *ram, CPU *cpu)
{
    // O resultado da divisao fica na RAM[3]
    Instrucao* trecho1 = (Instrucao*) malloc(3 * sizeof(Instrucao));
    
    trecho1[0].opcode = 3; // RAM[3] -> reg1
    trecho1[0].add1 = 1;
    trecho1[0].add2 = 3;

    trecho1[1].opcode = 5; // reg1 -> instrução
    trecho1[1].add1 = 1;
    trecho1[1].add2 = -1;
    trecho1[2].opcode = -1;
    
    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);
    
    return trecho1[1].add2;
}
