
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Instrucao.h"
#include "ram.h"
#include "cpu.h"
#include "programas.h"

#define TAM_RAM 50

void programaAleatorio(RAM *ram, CPU *cpu, int qdeIntrucoes)
{
    destroiRAM(ram);

    ram = criarRAM_aleatoria(TAM_RAM);

    Instrucao *umPrograma = (Instrucao *)malloc(qdeIntrucoes * sizeof(Instrucao));

    for (int i = 0; i < (qdeIntrucoes - 1); i++)
    {
        umPrograma[i].opcode = rand() % 2;
        umPrograma[i].add1 = rand() % TAM_RAM;
        umPrograma[i].add2 = rand() % TAM_RAM;
        umPrograma[i].add3 = rand() % TAM_RAM;
    }

    umPrograma[qdeIntrucoes - 1].opcode = -1;

    setPrograma(cpu, umPrograma);

    iniciar(ram, cpu);
}

void programaMult(RAM *ram, CPU *cpu, int multiplicando, int multiplicador)
{
    reinicializarRAM(ram, 2);

    Instrucao* trecho1 = (Instrucao*) malloc(3 * sizeof(Instrucao));

    // salva o multiplicando na RAM[1]
    trecho1[0].opcode = 4;
    trecho1[0].add1 = 1; // registrador1
    trecho1[0].add2 = multiplicando;

    trecho1[1].opcode = 2;
    trecho1[1].add1 = 1; // registrador1
    trecho1[1].add2 = 1; // ram[1]

    trecho1[2].opcode = -1; // opcode halt

    // envia e executa o primeiro trecho
    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);

    for (int i = 0; i < multiplicador; i++) // repete "multiplicador" vezes
    {
        // soma a ram[1] em ram[0]

        Instrucao* trecho2 = (Instrucao*) malloc(2 * sizeof(Instrucao));

        // soma ram[0] + ram[1]
        trecho2[0].opcode = 0;
        trecho2[0].add1 = 0; // ram[0]
        trecho2[0].add2 = 1; // ram[1]
        trecho2[0].add3 = 0; // ram[0]

        trecho2[1].opcode = -1; // opcode halt

        // executa o programa
        setPrograma(cpu, trecho2);
        iniciar(ram, cpu);
    }

    // traz o resultado pra fora da cpu
    Instrucao* trecho3 = (Instrucao*) malloc(3 * sizeof(Instrucao));

    // copia da ram[0] (resultado) para o registrador 1
    trecho3[0].opcode = 3; //
    trecho3[0].add1 = 1;   // registrador1
    trecho3[0].add2 = 0;   //

    trecho3[1].opcode = 5; //
    trecho3[1].add1 = 1;   // registrador1
    trecho3[1].add2 = -1;  //
    trecho3[1].add3 = -1;  //

    trecho3[2].opcode = -1;

    setPrograma(cpu, trecho3);
    iniciar(ram, cpu);

    printf("O resultado da multiplicacao e: %d\n", trecho3[1].add2);
}

// gcc -g -o exe *. c - Wall
// valgrind -- leak - check = full -s ./exe < casoteste . in
// valgrind-- tool = memcheck-- leak - check = full./

void programaDiv(RAM *ram, CPU *cpu, int dividendo, int divisor)
{
    reinicializarRAM(ram, 4);

    Instrucao* trecho1 = (Instrucao*) malloc(5 * sizeof(Instrucao));

    trecho1[0].opcode = 4;
    trecho1[0].add1 = 1; // registrador1
    trecho1[0].add2 = dividendo;

    trecho1[1].opcode = 2;
    trecho1[1].add1 = 1; // registrador1
    trecho1[1].add2 = 0; // ram[0]

    trecho1[2].opcode = 4;
    trecho1[2].add1 = 2; // registrador2
    trecho1[2].add2 = divisor;

    trecho1[3].opcode = 2;
    trecho1[3].add1 = 2; // registrador2
    trecho1[3].add2 = 1; // ram[1]

    trecho1[4].opcode = -1;

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);

    Instrucao* trecho2 = (Instrucao*) malloc(3 * sizeof(Instrucao));

    trecho2[0].opcode = 4;
    trecho2[0].add1 = 1; // registrador1
    trecho2[0].add2 = 1; // num 1

    trecho2[1].opcode = 2;
    trecho2[1].add1 = 1; // registrador1
    trecho2[1].add2 = 2; // ram[2]

    trecho2[2].opcode = -1;

    setPrograma(cpu, trecho2);
    iniciar(ram, cpu);

    while (dividendo >= divisor)
    {
        Instrucao* trecho3 = (Instrucao*) malloc(5 * sizeof(Instrucao));

        trecho3[0].opcode = 1; // sub
        trecho3[0].add1 = 0;   // ram[0]
        trecho3[0].add2 = 1;   // ram[1]
        trecho3[0].add3 = 0;   // ram[0]

        trecho3[1].opcode = 0; // sum
        trecho3[1].add1 = 2;   // ram[2]
        trecho3[1].add2 = 3;   // ram[3]
        trecho3[1].add3 = 3;   // ram[3]

        trecho3[2].opcode = 3;
        trecho3[2].add1 = 1;  // registrador1
        trecho3[2].add2 = 0;  // ram[0]
        trecho3[2].add3 = -1; // ram[1]

        trecho3[3].opcode = 5;
        trecho3[3].add1 = 1; // registrador1
        trecho3[3].add2 = -1;
        trecho3[3].add3 = -1;

        trecho3[4].opcode = -1;

        setPrograma(cpu, trecho3);
        iniciar(ram, cpu);

        dividendo = trecho3[3].add2;
    }

    Instrucao* trecho4 = (Instrucao*) malloc(3 * sizeof(Instrucao));    trecho4[0].opcode = 3;
    
    trecho4[0].add1 = 1; // registrador1
    trecho4[0].add2 = 3; // ram[3]
    trecho4[0].add3 = -1;

    trecho4[1].opcode = 5;
    trecho4[1].add1 = 1; // registrador1
    trecho4[1].add2 = -1;
    trecho4[1].add3 = -1;

    trecho4[2].opcode = -1;

    setPrograma(cpu, trecho4);
    iniciar(ram, cpu);

    printf("O resultado da divisao e: %d\n", trecho4[1].add2);
}

void programaFat(RAM *ram, CPU *cpu, int fat)
{
    // executar instrucao
    //-1 -> halt
    //  0 -> soma
    //  1 -> subtrai
    //  2 -> copia do registrador para RAM
    //  3 -> copia da RAM para o registrador
    //  4 -> salva conteudo externo no registrador
    //  5 -> obtem conteudo externo do registrador

    // 10! = 10x9x8x7x6x5x4x3x2x1

    reinicializarRAM(ram, 50);

    int j = 1;

    for (int i = 1; i <= fat; i++)
    {
        programaMult(ram, cpu, j, i);

        Instrucao* trecho1 = (Instrucao*) malloc(3 * sizeof(Instrucao));

        trecho1[0].opcode = 3;
        trecho1[0].add1 = 1; // registrador1
        trecho1[0].add2 = 0; // RAM[0]

        trecho1[1].opcode = 5;
        trecho1[1].add1 = 1; // registrador1
        trecho1[1].add2 = -1;
        trecho1[1].add3 = -1;

        trecho1[2].opcode = -1;

        setPrograma(cpu, trecho1);
        iniciar(ram, cpu);

        j = trecho1[1].add2;
    }

    Instrucao* trecho2 = (Instrucao*) malloc(3 * sizeof(Instrucao));

    trecho2[0].opcode = 3;
    trecho2[0].add1 = 1; // registrador1
    trecho2[0].add2 = 0; // ram[0]
    trecho2[0].add3 = -1;

    trecho2[1].opcode = 5;
    trecho2[1].add1 = 1; // registrador1
    trecho2[1].add2 = -1;
    trecho2[1].add3 = -1;

    trecho2[2].opcode = -1;

    setPrograma(cpu, trecho2);
    iniciar(ram, cpu);

    printf("O resultado do fatorial e: %d\n", trecho2[1].add2);
}

void programaSomaMatriz(RAM *ram, CPU *cpu, int cardinalidade)
{
    // Calcula o tamanho necessário para a RAM
    int n_elementos = cardinalidade * cardinalidade;
    int tamanhoRAM = n_elementos * 3; // matriz1, matriz2, resultado

    int (*matriz1)[cardinalidade] = malloc(sizeof(int[cardinalidade][cardinalidade]));
    int (*matriz2)[cardinalidade] = malloc(sizeof(int[cardinalidade][cardinalidade]));

    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            matriz1[i][j] = rand() % 100;
            matriz2[i][j] = rand() % 100;
        }
    }

    printf("--- Matriz 1 (Original) ---\n");
    imprimirMatriz(cardinalidade, cardinalidade, matriz1);
    printf("--- Matriz 2 (Original) ---\n");
    imprimirMatriz(cardinalidade, cardinalidade, matriz2);

    reinicializarRAM(ram, tamanhoRAM);
    
    int endRam = 0;
    
    // carrega Matriz 1 na RAM
    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            
            Instrucao* trecho1 = (Instrucao*) malloc(3 * sizeof(Instrucao));
            
            trecho1[0].opcode = 4;
            trecho1[0].add1 = 1; // registrador1
            trecho1[0].add2 = matriz1[i][j];

            trecho1[1].opcode = 2;
            trecho1[1].add1 = 1;      // registrador1
            trecho1[1].add2 = endRam; // ram[endRam]

            trecho1[2].opcode = -1;

            setPrograma(cpu, trecho1);
            iniciar(ram, cpu);
            endRam++;
        }
    }

    // carrega Matriz 2 na RAM  endRam continua de onde parou
    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            
            Instrucao* trecho2 = (Instrucao*) malloc(3 * sizeof(Instrucao));
            
            trecho2[0].opcode = 4;
            trecho2[0].add1 = 1; // registrador1
            trecho2[0].add2 = matriz2[i][j];

            trecho2[1].opcode = 2;
            trecho2[1].add1 = 1;      // registrador1
            trecho2[1].add2 = endRam; // ram[endRam]

            trecho2[2].opcode = -1;

            setPrograma(cpu, trecho2);
            iniciar(ram, cpu);

            endRam++;
        }
    }
    
    // soma dentro ad ram
    
    endRam = 0; // Reinicia o contador de endereço
    
    int delta = n_elementos; 

    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            
            Instrucao* trecho3 = (Instrucao*) malloc(2 * sizeof(Instrucao));
            
            trecho3[0].opcode = 0;                  // sum
            trecho3[0].add1 = endRam;               // ram[M1]
            trecho3[0].add2 = endRam + delta;       // ram[M2]
            trecho3[0].add3 = endRam + (2 * delta); // ram[M_Resultado]

            trecho3[1].opcode = -1;

            setPrograma(cpu, trecho3);
            iniciar(ram, cpu);

            endRam++;
        }
    }

    free(matriz1);
    free(matriz2);

    printf("--- Conteúdo Final da RAM (M1, M2, M_Resultado) ---\n");
    imprimirRAM(ram);
}

void imprimirMatriz(int rows, int cols, int matrix[rows][cols])
{
    printf("--- Imprimindo Matriz (%dx%d) ---\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Imprime o número e um 'tab' para alinhamento
            printf("%d\t", matrix[i][j]);
        }
        // Pula uma linha no final de cada linha da matriz
        printf("\n");
    }
    printf("----------------------------------\n");
}