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

    /*  RAM[0]: resultado
        RAM[1]: multiplicando */

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
        Soma(ram,cpu,0,1,0);
    }

    printf("O resultado da multiplicacao e: %d\n", pegarMult(ram,cpu));
}

// gcc -g -o exe *. c - Wall
// valgrind -- leak - check = full -s ./exe < casoteste . in
// valgrind-- tool = memcheck-- leak - check = full./

void programaDiv(RAM *ram, CPU *cpu, int dividendo, int divisor)
{
    /*
        Ram 0 = dividendo
        ram 1 = divisor
        ram 2 = 1 (para somar resultado)
        ram 3 = resultado
    */

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
        
        Subtrai(ram,cpu,0,1,0);
        
        Soma(ram,cpu,2,3,3);
        
        Instrucao* trecho3 = (Instrucao*) malloc(3 * sizeof(Instrucao));

        trecho3[0].opcode = 3;
        trecho3[0].add1 = 1;  // registrador1
        trecho3[0].add2 = 0;  // ram[0]
        trecho3[0].add3 = -1; // ram[1]

        trecho3[1].opcode = 5;
        trecho3[1].add1 = 1; // registrador1
        trecho3[1].add2 = -1;
        trecho3[1].add3 = -1;

        trecho3[2].opcode = -1;

        setPrograma(cpu, trecho3);
        iniciar(ram, cpu);

        dividendo = trecho3[1].add2;
    }

    printf("O resultado da divisao e: %d\n", pegarDiv(ram,cpu));
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

        j = pegarMult(ram,cpu);
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

    printf("Matriz 1\n");
    imprimirMatriz(cardinalidade, cardinalidade, matriz1);
    printf("Matriz 2\n");
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

    int (*matriz3)[cardinalidade] = malloc(sizeof(int[cardinalidade][cardinalidade]));
    int k = 8;
    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            matriz3[i][j] = getDado(ram,k);
            k++;
        }
    }

    printf("\nMatriz 3\n");
    imprimirMatriz(cardinalidade, cardinalidade, matriz3);

    free(matriz1);
    free(matriz2);
    free(matriz3);

    imprimirRAM(ram);
}

void programaRaizQuadrada(RAM *ram, CPU *cpu, int numero)
{
    // utilizar teoria dos numeros primos
    // a raiz quadrada de um numero, é a quantidade de operações  de subtrações que sao feitas
    // por meio de uma crescente de numeros primos 1,3,5,7
    // os quais sao incrementados de 2 a 2 

    // RAM[0]: N , o resto
    // RAM[1]: O ímpar atual 
    // RAM[2]: O contador , que é or esultado
    // RAM[3]: Constante '1' para incrementar o contador, igual o joubert fez
    // RAM[4]: Constante '2' para incrementar o ímpar

    reinicializarRAM(ram, 5);

    
    Instrucao* trecho1 = (Instrucao*) malloc(8 * sizeof(Instrucao));

    // colocar o n na posilçao 0
    trecho1[0].opcode = 4; // ext->reg
    trecho1[0].add1 = 1;
    trecho1[0].add2 = numero; // passo o valor pra registrador

    trecho1[1].opcode = 2; // reg->RAM
    trecho1[1].add1 = 1; // registrador para ram
    trecho1[1].add2 = 0; // RAM[0]

    // colcoar '1' 
    trecho1[2].opcode = 4; // ext->reg
    trecho1[2].add1 = 1; 
    trecho1[2].add2 = 1; //passo valor pra registrador

    trecho1[3].opcode = 2; // reg->RAM
    trecho1[3].add1 = 1;
    trecho1[3].add2 = 1; // RAM[1]

    trecho1[4].opcode = 2; // reg->RAM 
    trecho1[4].add1 = 1; // aproveito o registrador e mando para a ram[3]
    trecho1[4].add2 = 3; // RAM[3]

    //  coloco o 2 agr
    trecho1[5].opcode = 4; // ext->reg
    trecho1[5].add1 = 1;  
    trecho1[5].add2 = 2; 

    trecho1[6].opcode = 2; // reg->RAM
    trecho1[6].add1 = 1; 
    trecho1[6].add2 = 4; // RAM[4]

    trecho1[7].opcode = -1; 
    

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);


    int restante = numero;
    int impar = 1;

    while (restante >= impar)
    {
        // joao, é nessa parte que acontece os calculos     

        // RAM[0] = RAM[0] - RAM[1] (N -= ímpar)
        Subtrai(ram,cpu,0,1,0);
        
        Soma(ram,cpu,1,4,1);
       
        // RAM[2] = RAM[2] + RAM[3] (resultado += 1)
        Soma(ram,cpu,2,3,1);

        Instrucao* trecho3 = (Instrucao*) malloc(3 * sizeof(Instrucao));

        trecho3[0].opcode = 3; // RAM[0] -> reg1
        trecho3[0].add1 = 1;
        trecho3[0].add2 = 0;

        trecho3[1].opcode = 5; // reg1 -> instrução
        trecho3[1].add1 = 1;
        trecho3[1].add2 = -1;
        trecho3[2].opcode = -1;

        setPrograma(cpu, trecho3);
        iniciar(ram, cpu);

        restante = trecho3[1].add2;

        Instrucao* trecho4 = (Instrucao*) malloc(3 * sizeof(Instrucao));

        trecho4[0].opcode = 3; // RAM[1] -> reg1
        trecho4[0].add1 = 1;
        trecho4[0].add2 = 1;

        trecho4[1].opcode = 5; // reg1 -> instrução
        trecho4[1].add1 = 1;
        trecho4[1].add2 = -1;
        trecho4[2].opcode = -1;

        setPrograma(cpu, trecho4);
        iniciar(ram, cpu);
        impar = trecho4[1].add2;
    }

    // buscar o resultado que está em RAM[2] 
    Instrucao* trecho5 = (Instrucao*) malloc(3 * sizeof(Instrucao));
    trecho5[0].opcode = 3; // RAM[2] -> reg1
    trecho5[0].add1 = 1;
    trecho5[0].add2 = 2;

    trecho5[1].opcode = 5; // reg1 -> instrução
    trecho5[1].add1 = 1;
    trecho5[1].add2 = -1;
    trecho5[2].opcode = -1;

    setPrograma(cpu, trecho5);
    iniciar(ram, cpu);

    printf("A raiz quadrada de %d e: %d\n", numero, trecho5[1].add2);
}

void programaRaizCubica(RAM *ram, CPU *cpu, int numero)
{

    /*A logica da raiz cubica se parece com a da raiz quadrada, pois é uma sucessivade de subtrações,
    de numeros que sao logicamentes incrementados
    */

    // Layout da RAM:
    // RAM[0]: N (o restante)
    // RAM[1]: O "decremento" (1, 7, 19, 37...)
    // RAM[2]: A "Diferenca" (6, 12, 18...)
    // RAM[3]: Constante '6'
    // RAM[4]: O contador (o resultado)
    // RAM[5]: Constante '1'
    
    reinicializarRAM(ram, 6); 

    Instrucao* trecho1 = (Instrucao*) malloc(9 * sizeof(Instrucao));

    // numero em RAM[0]
    trecho1[0].opcode = 4; // ext->reg
    trecho1[0].add1 = 1;
    trecho1[0].add2 = numero;

    trecho1[1].opcode = 2; // reg->RAM
    trecho1[1].add1 = 1;
    trecho1[1].add2 = 0; // RAM[0]

    // 1 em RAM[1] que é o decremento ,RAM[5]  1, constante de aumento do resultado
    trecho1[2].opcode = 4; // ext->reg
    trecho1[2].add1 = 1;
    trecho1[2].add2 = 1;

    trecho1[3].opcode = 2; // reg->RAM
    trecho1[3].add1 = 1;
    trecho1[3].add2 = 1; // RAM[1]

    trecho1[4].opcode = 2; // reg->RAM
    trecho1[4].add1 = 1; // aproveita o msm registrador
    trecho1[4].add2 = 5; // RAM[5]

    // 6 em RAM[2] que é a primeira diferença,  e RAM[3]  6 a constante que aumenta a diferença
    trecho1[5].opcode = 4; // ext->reg
    trecho1[5].add1 = 1;
    trecho1[5].add2 = 6;
    trecho1[6].opcode = 2; // reg->RAM
    trecho1[6].add1 = 1;
    trecho1[6].add2 = 2; // RAM[2]

    trecho1[7].opcode = 2; // reg->RAM
    trecho1[7].add1 = 1; // aproveitamos o msm dnv
    trecho1[7].add2 = 3; // RAM[3]
    
    // Resultado que ficaria em ram 4, ja é zero peloc calloc
    trecho1[8].opcode = -1; 

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);

    int restante = numero;
    int decremento = 1;

    while (restante >= decremento)
        {
        // dentro do while que de fato sao feito as operações

            Instrucao* trecho2 = (Instrucao*) malloc(5 * sizeof(Instrucao));

            // RAM[0] = RAM[0] - RAM[1] (N -= decremento)
            trecho2[0].opcode = 1;
            trecho2[0].add1 = 0;
            trecho2[0].add2 = 1;
            trecho2[0].add3 = 0;

            // RAM[1] = RAM[1] + RAM[2] (decremento += diferença )
            trecho2[1].opcode = 0;
            trecho2[1].add1 = 1; // [add1=1]
            trecho2[1].add2 = 2; // [add2=2]
            trecho2[1].add3 = 1; // [add3=1]

            // RAM[2] = RAM[2] + RAM[3] (difenreça += 6) que vai ser a proxima diferemça
            trecho2[2].opcode = 0;
            trecho2[2].add1 = 2; // [add1=2]
            trecho2[2].add2 = 3; // [add2=3]
            trecho2[2].add3 = 2; // [add3=2]

            // RAM[4] = RAM[4] + RAM[5] (Resultado += 1)
            trecho2[3].opcode = 0;
            trecho2[3].add1 = 4;
            trecho2[3].add2 = 5;
            trecho2[3].add3 = 4;

            trecho2[4].opcode = -1; 

            setPrograma(cpu, trecho2);
            iniciar(ram, cpu);

            
            Instrucao* trecho3 = (Instrucao*) malloc(3 * sizeof(Instrucao));

            trecho3[0].opcode = 3; // RAM[0] -> reg1
            trecho3[0].add1 = 1;
            trecho3[0].add2 = 0;

            trecho3[1].opcode = 5; // reg1 -> instrução
            trecho3[1].add1 = 1;
            trecho3[1].add2 = -1;
            
            trecho3[2].opcode = -1;

            setPrograma(cpu, trecho3);
            iniciar(ram, cpu);
            // aqui eu rodei a cpu, pra epgar o valor do restante novo, pra testa no while
            restante = trecho3[1].add2;

            Instrucao* trecho4 = (Instrucao*) malloc(3 * sizeof(Instrucao));
            trecho4[0].opcode = 3; // RAM[1] -> reg1
            trecho4[0].add1 = 1;
            trecho4[0].add2 = 1;

            trecho4[1].opcode = 5; // reg1 -> instrução
            trecho4[1].add1 = 1;
            trecho4[1].add2 = -1;

            trecho4[2].opcode = -1;
            setPrograma(cpu, trecho4);
            iniciar(ram, cpu);

    // aqui eu rodei a cpu, pra epgar o valor do decremento novo, pra testa no while

            decremento = trecho4[1].add2;
        }

    // aqui é pra tirar o resultado da ram
    Instrucao* trecho5 = (Instrucao*) malloc(3 * sizeof(Instrucao));
    
    trecho5[0].opcode = 3; // RAM[4] -> reg1
    trecho5[0].add1 = 1;
    trecho5[0].add2 = 4;
    
    trecho5[1].opcode = 5; // reg1 -> instrução
    trecho5[1].add1 = 1;
    trecho5[1].add2 = -1;
    
    trecho5[2].opcode = -1;

    setPrograma(cpu, trecho5);
    iniciar(ram, cpu);

    printf("A raiz cubica de %d e: %d\n", numero, trecho5[1].add2);
}

void programaRaioEsfera(RAM *ram, CPU *cpu, int volume)
{
    // nao reinicializei a ram , pq o primeiro programa ja faz isso
    // eu assumi que pi é 3, ai a formula fica r ao cubo = v/4
    // Calcular Volume / 4

    programaDiv(ram, cpu, volume, 4);

    // agora calcular a raiz do volume dividido
    programaRaizCubica(ram, cpu, pegarDiv(ram,cpu));

    // o resultado ta na ram 
    // agr tem q acessar la dnv   
    
    Instrucao* trecho2 = (Instrucao*) malloc(3 * sizeof(Instrucao));
    
    trecho2[0].opcode = 3; // RAM[4] -> reg1
    trecho2[0].add1 = 1;
    trecho2[0].add2 = 4; // Posição do resultado da raiz

    trecho2[1].opcode = 5; // reg1 -> instrução
    trecho2[1].add1 = 1;
    trecho2[1].add2 = -1;
    
    trecho2[2].opcode = -1; 
    
    setPrograma(cpu, trecho2);
    iniciar(ram, cpu);

    printf("O raio com PI = 3  da esfera com o volume de %d e: %d\n", volume, trecho2[1].add2);
}

void programaBin_Dec(RAM *ram, CPU *cpu, char *binario) // passo em char porque ai ja tenho ele separado, 
//dava pra passar um numero e usar o codigo de separar, mas nao tem necessidade
{
    /* OPCODE
    -1 = Halt
    0 soma
    1 subtrai
    2 copia do registrador para RAM
    3 copia da RAM para o registrador
    4 salva conteudo externo no registrador
    5 obtem conteudo externo do registrador
    */

    // Layout da RAM:
    // RAM[0]: O "Total" (Resultado, comeca em 0)
    // RAM[1]: O "Peso" (Comeca em 1, depois 2, 4, 8...)
    reinicializarRAM(ram, 2);

    // --- Trecho 1: Setup (Colocar '1' na RAM[1]) ---
    Instrucao *trecho1 = (Instrucao *)malloc(3 * sizeof(Instrucao));

    trecho1[0].opcode = 4; // ext->reg1
    trecho1[0].add1 = 1;
    trecho1[0].add2 = 1; // O número 1

    trecho1[1].opcode = 2; // reg1->RAM[1]
    trecho1[1].add1 = 1;
    trecho1[1].add2 = 1;

    trecho1[2].opcode = -1; // Halt

    setPrograma(cpu, trecho1);
    iniciar(ram, cpu);


    int tamanho = strlen(binario);

    // pega  do fim da string para o comeo
    for (int i = (tamanho - 1); i >= 0; i--)
    {
        char bit_atual = binario[i];

        // Se o digito for 1, somamos o peso ao total
        if (bit_atual == '1')
        {
            Instrucao *trecho2 = (Instrucao *)malloc(2 * sizeof(Instrucao));

            // RAM[0] = RAM[0] + RAM[1] ; Total = Total + Peso
            trecho2[0].opcode = 0;
            trecho2[0].add1 = 0;
            trecho2[0].add2 = 1;
            trecho2[0].add3 = 0;

            trecho2[1].opcode = -1; // Halt

            setPrograma(cpu, trecho2);
            iniciar(ram, cpu);
        }

        // dobra  para a proxima iteracao
        Instrucao *trecho3 = (Instrucao *)malloc(2 * sizeof(Instrucao));

        // RAM[1] = RAM[1] + RAM[1] ; peso = peso *2 
        trecho3[0].add1 = 1;
        trecho3[0].add2 = 1;
        trecho3[0].add3 = 1;

        trecho3[1].opcode = -1; // Halt

        setPrograma(cpu, trecho3);
        iniciar(ram, cpu);
    }

    //pegar o result
    Instrucao *trecho4 = (Instrucao *)malloc(3 * sizeof(Instrucao));

    trecho4[0].opcode = 3; // RAM[0] -> reg1
    trecho4[0].add1 = 1;
    trecho4[0].add2 = 0;

    trecho4[1].opcode = 5; // reg1 -> instrução
    trecho4[1].add1 = 1;
    trecho4[1].add2 = -1;

    trecho4[2].opcode = -1; 

    setPrograma(cpu, trecho4);
    iniciar(ram, cpu);

    printf("O resultado decimal de '%s' e: %d\n", binario, trecho4[1].add2);
}


























































void programaAreaQuadrado(RAM *ram, CPU *cpu, int lado)
{
    programaMult(ram, cpu, lado, lado);
    
    printf(">>> A area do quadrado e: %d\n", pegarMult(ram,cpu));
}



void programaAreaTriangulo(RAM *ram, CPU *cpu, int base, int altura)
{
    programaMult(ram, cpu, base, altura);
    
    int produto = pegarMult(ram,cpu);
    
    programaDiv(ram, cpu, produto, 2);
    
    printf("A area do triangulo e: %d\n", pegarDiv(ram,cpu));
}


void programaAreaCirculo(RAM *ram, CPU *cpu, int raio)
{
    programaMult(ram, cpu, raio, raio);
    
    int raioQuadrado = pegarMult(ram,cpu);

    programaMult(ram, cpu, 3, raioQuadrado);
        
    printf("A area aproximada do circulo e: %d\n", pegarMult(ram,cpu));
}

void programaCircunferenciaCirculo(RAM *ram, CPU *cpu, int raio)
{
    // 2 *3 = 6        6* raio = c
    programaMult(ram, cpu, 6, raio);

    printf("A circunferencia aproximada do circulo e: %d\n", pegarMult(ram,cpu));
}

void programaVolumeCaixa(RAM *ram, CPU *cpu, int comprimento, int largura, int altura)
{
   
    programaMult(ram, cpu, comprimento, largura);

    int areaBase = pegarMult(ram, cpu);

    programaMult(ram, cpu, areaBase, altura);
    
    printf("O volume da caixa e: %d\n", pegarMult(ram, cpu));
}


void programaAreaTrapezio(RAM *ram, CPU *cpu, int baseMaior, int baseMenor, int altura)
{   
    int produto_soma = (baseMaior + baseMenor) * altura;

    programaDiv(ram, cpu, produto_soma, 2);

    printf("A area do trapezio e: %d\n", pegarDiv(ram, cpu));
}



void programaVolumeCilindro(RAM *ram, CPU *cpu, int raio, int altura)
{

    programaMult(ram, cpu, raio, raio);

    int raioQuadrado = pegarMult(ram, cpu);

    programaMult(ram, cpu, raioQuadrado, 3);

    int areaBase = pegarMult(ram, cpu);

    programaMult(ram, cpu, areaBase, altura);
    
    printf("O volume aproximado do cilindro e: %d\n", pegarMult(ram, cpu));
}


