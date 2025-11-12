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

srand(time(NULL));
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

    /// salva o multiplicando na RAM[1]
    colocarNaRam(ram, cpu, 1, multiplicando);

    for (int i = 0; i < multiplicador; i++) // repete "multiplicador" vezes
    {
        Soma(ram, cpu, 0, 1, 0);
    }

    printf("O resultado da multiplicacao e: %d\n", pegarMult(ram, cpu));
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

    // colocando dividendo na ram 0
    colocarNaRam(ram, cpu, 0, dividendo);

    // colocando divisor na ram 1
    colocarNaRam(ram, cpu, 1, divisor);

    // colocar 1 na ram 2
    colocarNaRam(ram, cpu, 2, 1);

    while (dividendo >= divisor)
    {

        Subtrai(ram, cpu, 0, 1, 0);

        Soma(ram, cpu, 2, 3, 3);

        dividendo = pegarResultado(ram, cpu, 0);
    }

    printf("O resultado da divisao e: %d\n", pegarDiv(ram, cpu));
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

        j = pegarMult(ram, cpu);
    }

    printf("O resultado do fatorial e: %d\n",j);
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

            colocarNaRam(ram, cpu, endRam, matriz1[i][j]);
            endRam++;
        }
    }

    // carrega Matriz 2 na RAM  endRam continua de onde parou
    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {

            colocarNaRam(ram, cpu, endRam, matriz2[i][j]);
            endRam++;
        }
    }

    // soma dentro ad ram

    endRam = 0; // Reinicia o contador de endereco

    int delta = n_elementos;

    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {

            Soma(ram, cpu, endRam, endRam + delta, endRam + (2 * delta));
            endRam++;
        }
    }

    int (*matriz3)[cardinalidade] = malloc(sizeof(int[cardinalidade][cardinalidade]));
    int k = 8;
    for (int i = 0; i < cardinalidade; i++)
    {
        for (int j = 0; j < cardinalidade; j++)
        {
            matriz3[i][j] = getDado(ram, k);
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
    // RAM[2]: O contador , que é o esultado
    // RAM[3]: Constante '1' para incrementar o contador, igual o joubert fez
    // RAM[4]: Constante '2' para incrementar o ímpar

    reinicializarRAM(ram, 5);

    colocarNaRam(ram, cpu, 0, numero); // N em RAM[0]

    colocarNaRam(ram, cpu, 1, 1); // 1 em RAM[1] (impar)

    colocarNaRam(ram, cpu, 3, 1); // 1 em RAM[3]

    colocarNaRam(ram, cpu, 4, 2); // 2 em RAM[4]

    int restante = numero;
    int impar = 1;

    while (restante >= impar)
    {
        // joao, é nessa parte que acontece os calculos

        // RAM[0] = RAM[0] - RAM[1] (N -= ímpar)
        Subtrai(ram, cpu, 0, 1, 0);
        // aumentando o impar
        Soma(ram, cpu, 1, 4, 1);

        // RAM[2] = RAM[2] + RAM[3] (resultado += 1)
        Soma(ram, cpu, 2, 3, 2);

        restante = pegarResultado(ram, cpu, 0);

        impar = pegarResultado(ram, cpu, 1);
    }

    printf("A raiz quadrada de %d e: %d\n", numero, pegarResultado(ram, cpu, 2));
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

    colocarNaRam(ram, cpu, 0, numero); // N em RAM[0]

    colocarNaRam(ram, cpu, 1, 1); // 1 em RAM[1]

    colocarNaRam(ram, cpu, 5, 1); // 1 em RAM[5]

    colocarNaRam(ram, cpu, 2, 6); // 6 em RAM[2]

    colocarNaRam(ram, cpu, 3, 6); // 6 em RAM[3]

    int restante = numero;
    int decremento = 1;

    while (restante >= decremento)
    {
        // dentro do while que de fato sao feito as operações
        // RAM[0] = RAM[0] - RAM[1] (N -= decremento)
        Subtrai(ram, cpu, 0, 1, 0);

        // RAM[1] = RAM[1] + RAM[2] (decremento += diferença )
        Soma(ram, cpu, 1, 2, 1);
        // RAM[2] = RAM[2] + RAM[3] (diferença += 6)
        Soma(ram, cpu, 2, 3, 2);
        // RAM[4] = RAM[4] + RAM[5] (Resultado += 1)
        Soma(ram, cpu, 4, 5, 4);

        restante = pegarResultado(ram, cpu, 0);

        decremento = pegarResultado(ram, cpu, 1);
    }

    printf("A raiz cubica de %d e: %d\n", numero, pegarResultado(ram, cpu, 4));
}

void programaRaioEsfera(RAM *ram, CPU *cpu, int volume)
{
    // nao reinicializei a ram , pq o primeiro programa ja faz isso
    // eu assumi que pi é 3, ai a formula fica r ao cubo = v/4
    // Calcular Volume / 4

    programaDiv(ram, cpu, volume, 4);

    // agora calcular a raiz do volume dividido
    programaRaizCubica(ram, cpu, pegarDiv(ram, cpu));

    // o resultado ta na ram
    // agr tem q acessar la dnv

    printf("O raio com PI = 3  da esfera com o volume de %d e: %d\n", volume, pegarResultado(ram, cpu, 4));
}

void programaDivFloat2(RAM *ram, CPU *cpu, int dividendo, int divisor)
{

    programaMult(ram, cpu, dividendo, 100);

    int dividendo_100 = pegarMult(ram, cpu);

    programaDiv(ram, cpu, dividendo_100, divisor);

    int resultado_100 = pegarDiv(ram, cpu);

    programaDiv(ram, cpu, resultado_100, 100);

    int parte_inteira = pegarDiv(ram, cpu);
    
    programaRestoDiv(ram, cpu, resultado_100, 100);

    colocarNaRam(ram,cpu,1,parte_inteira);

    int parte_decimal = pegarResultado(ram, cpu, 0);

    printf("O resultado da divisao e: %d.%02d\n", parte_inteira, parte_decimal);
}

void programaMMC(RAM *ram, CPU *cpu, int a, int b)
{

    // calcular a * b
    programaMult(ram, cpu, a, b);

    int produto = pegarMult(ram, cpu);

    // calcular MDC(a, b) ---
    programaMdc(ram, cpu, a, b);

    int mdc = pegarResultado(ram, cpu, 8);

    programaDiv(ram, cpu, produto, mdc);

    int mmc = pegarDiv(ram, cpu);

    printf("O MMC e: %d\n", mmc);
}

void programaBin_Dec(RAM *ram, CPU *cpu, char *binario)
// passo em char porque ai ja tenho ele separado,
// dava pra passar um numero e usar o codigo de separar, mas nao tem necessidade
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

    // Colocar '1' na RAM[1]) ---
    colocarNaRam(ram, cpu, 1, 1);

    int tamanho = strlen(binario);

    // pega  do fim da string para o comeco
    for (int i = (tamanho - 1); i >= 0; i--)
    {
        char bit_atual = binario[i];

        // Se o digito for 1, somamos o peso ao total
        if (bit_atual == '1')
        {
            // RAM[0] = RAM[0] + RAM[1] ; Total = Total + Peso
            Soma(ram, cpu, 0, 1, 0);
        }

        // dobra  para a proxima iteracao
        Soma(ram, cpu, 1, 1, 1);
    }

    printf("O resultado decimal de '%s' e: %d\n", binario, pegarResultado(ram, cpu, 0));
}

void programaDec_Bin(RAM *ram, CPU *cpu, int decimal)
{
    if (decimal == 0)
    {
        printf("O resultado binario e: 0\n");
        return;
    }

    char bitsInvertidos[65];
    int i = 0;

    while (decimal > 0)
    {
        programaRestoDiv(ram, cpu, decimal, 2);

        int resto = pegarResultado(ram, cpu, 0);

        if (resto == 1)
        {
            bitsInvertidos[i] = '1';
        }
        else
        {
            bitsInvertidos[i] = '0';
        }

        i++;

        // resultado da divisao
        decimal = pegarDiv(ram, cpu);
    }

    // Termina a string
    bitsInvertidos[i] = '\0';

    inverterString(bitsInvertidos);

    printf("O resultado binario e: %s\n", bitsInvertidos);
}

void programaHex_Bin(RAM *ram, CPU *cpu,char *hex) {
    
    // cada hex * 4 + \0
    char resultadoBin[strlen(hex) * 4 + 1];
    
    // para pd concatenar
    resultadoBin[0] = '\0'; 

    for(int i = 0; i < strlen(hex); i++) {
        // Pega o caractere atual (ex: 'A')
        char charAtual = hex[i];
        
        // Mapeia para os 4 bits (ex: "1010")
        char* bits = HexParaBin(charAtual);
        
        //colca de tras pra frente
        strcat(resultadoBin, bits);
    }

    programaBin_Dec(ram,cpu,resultadoBin);

    printf("O resultado binario e: %s\n", resultadoBin);
}

void programaHex_Dec(RAM *ram, CPU *cpu,char *hex){
    programaHex_Bin(ram,cpu,hex);
    printf("O resultado decimal e: %d\n",pegarResultado(ram, cpu, 0));
}


void programaBin_Hex(RAM *ram, CPU *cpu,char *binario) {


    int tam = strlen(binario);
    
    // olha zero a esquerda pra multiplo de 4
    int preenchimento = (4 - (tam % 4)) % 4; // haxadecimais funcionam em multiplos de 5
    
    char binPreenchido[tam + preenchimento + 1]; // pra ter o tamanho total

    //  ele salva o novo numero q varia do preenchumaneto, coloca zeros antes 
    sprintf(binPreenchido, "%.*s%s", preenchimento, "0000", binario); // Ex: "00110101"
    
    char resultaHex[strlen(binPreenchido) / 4 + 1];
    
    int indiceHex = 0;


    for(int i = 0; i < strlen(binPreenchido); i += 4) {
        
        char pedaco[5];
        strncpy(pedaco, &binPreenchido[i], 4); // ele salvar os pedaços de i + 3 posições em 'pedaco'
        pedaco[4] = '\0'; // Termina a string do pedaço
        
        programaBin_Dec(ram,cpu,pedaco);// trasnforma

        int valorInt = pegarResultado(ram, cpu, 0);
        
        resultaHex[indiceHex] =  DecParaHex(valorInt);// e tranforma denovo
        indiceHex++;
    }
    
    resultaHex[indiceHex] = '\0'; // Termina a string final

    printf("O resultado hexadecimal e: %s\n", resultaHex);
}

void programaDec_Hex(RAM *ram, CPU *cpu, int decimal)
{
    printf("Iniciando conversao de %d para hexadecimal:\n", decimal);

    if (decimal == 0) {
        printf("O resultado hexadecimal e: 0\n");
        return;
    }
    char bitsInvertidos[65];
    int i = 0;


    while (decimal > 0)
    {
        // msm logica do binario mas na base 16
        programaRestoDiv(ram, cpu, decimal, 16);

        int resto = pegarResultado(ram, cpu, 0);

        bitsInvertidos[i] = DecParaHex(resto);
        i++;

        decimal = pegarDiv(ram, cpu);
    }

    bitsInvertidos[i] = '\0';

    inverterString(bitsInvertidos);

    printf("O resultado hexadecimal e: %s\n", bitsInvertidos);
}

void programaExpBinaria(RAM *ram, CPU *cpu, int base, int expoente)
{
    
    int resultado_C = 1;     // Guarda o resultado final
  
    if (expoente == 0)
    {
        printf("O resultado da exponenciacao e: 1\n");
        return;
    }

    while (expoente > 0)
        {
           // 3 ^10 = 3 ^1010 = 3^8 * 3^2 que sao as posições dos binarios
        programaDiv(ram, cpu, expoente, 2);
       
        int resto = pegarResultado(ram, cpu, 0); // Este é o bit (expoente % 2)
        int novo_exp = pegarDiv(ram, cpu);      // Este é o novo expoente (expoente / 2)

        // 3. Se o bit for 1 (expoente era ímpar)
        if (resto == 1){ 
            
            programaMult(ram, cpu, resultado_C, base);
            
            resultado_C = pegarMult(ram, cpu); // C salva o novo resultado
        }

        programaMult(ram, cpu, base, base); //aqui é como se dobrassemos o expoente toda vez
        base = pegarMult(ram, cpu); // C salva a nova base

        expoente = novo_exp;
    }

    printf("O resultado da exponenciacao e: %d\n", resultado_C);
}

void programaSomaPA(RAM *ram, CPU *cpu, int a1, int razao, int n)
{

    programaPA(ram, cpu, a1, razao, n);
    
    int aN = pegarResultado(ram,cpu,0);

    colocarNaRam(ram,cpu,0,a1);

    colocarNaRam(ram,cpu,1,aN);
    
    Soma(ram,cpu,0,1,0);

    programaMult(ram, cpu, pegarResultado(ram,cpu,0), n);

    int produto = pegarMult(ram, cpu);
  
    programaDiv(ram, cpu, produto, 2);

    printf("O Somatorio da PA e: = %d\n", pegarDiv(ram,cpu));

}



void programaAreaQuadrado(RAM *ram, CPU *cpu, int lado)
{
    programaMult(ram, cpu, lado, lado);

    printf("A area do quadrado e: %d\n", pegarMult(ram, cpu));
}

void programaAreaTriangulo(RAM *ram, CPU *cpu, int base, int altura)
{
    programaMult(ram, cpu, base, altura);

    int produto = pegarMult(ram, cpu);

    programaDiv(ram, cpu, produto, 2);

    printf("A area do triangulo e: %d\n", pegarDiv(ram, cpu));
}

void programaAreaCirculo(RAM *ram, CPU *cpu, int raio)
{
    programaMult(ram, cpu, raio, raio);

    int raioQuadrado = pegarMult(ram, cpu);

    programaMult(ram, cpu, 3, raioQuadrado);

    printf("A area aproximada do circulo e: %d\n", pegarMult(ram, cpu));
}

void programaCircunferenciaCirculo(RAM *ram, CPU *cpu, int raio)
{
    // 2 *3 = 6        6* raio = c
    programaMult(ram, cpu, 6, raio);

    printf("A circunferencia aproximada do circulo e: %d\n", pegarMult(ram, cpu));
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

    reinicializarRAM(ram, 2); 
    
    colocarNaRam(ram, cpu, 0, baseMaior);
    colocarNaRam(ram, cpu, 1, baseMenor);
    
    Soma(ram, cpu, 0, 1, 0);
    
    int somaBases = pegarResultado(ram, cpu, 0);

    programaMult(ram, cpu, somaBases, altura);
    
    int produto = pegarMult(ram, cpu);
    
    programaDiv(ram, cpu, produto, 2);
    
    int area = pegarDiv(ram, cpu);

    printf("A area do trapezio e: %d\n", area);
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

// JOAO

/* OPCODE
    -1 = Halt
    0 soma
    1 subtrai
    2 copia do registrador para RAM
    3 copia da RAM para o registrador
    4 salva conteudo externo no registrador
    5 obtem conteudo externo do registrador
    */

void programaPA(RAM *ram, CPU *cpu, int a1, int razao, int n)
{
    printf("Iniciando Calculo da PA: A(n) = %d +(%d-1)*%d\n", a1, n, razao);

    // Calculando (n-1)
    reinicializarRAM(ram, 3); // limpa 3 posições da ram

    // RAM[0] = n
    colocarNaRam(ram, cpu, 0, n);

    // RAM[1] = 1
    colocarNaRam(ram, cpu, 1, 1);

    // RAM[2]=(n-1)
    printf("Conta: (%d - 1)\n", n);
    Subtrai(ram, cpu, 0, 1, 2);

    // Pegando resultado de (n-1)
    int resultado = pegarResultado(ram, cpu, 2);

    printf("Resultado = %d\n", resultado);

    // Calculando (n-1) * razao
    printf("Conta: (%d * %d)\n", resultado, razao);
    programaMult(ram, cpu, resultado, razao); // RAM[0] = (n-1) * razao

    // RAM[1] = a1
    colocarNaRam(ram, cpu, 1, a1);

    printf("Conta: (%d + %d)\n", a1, pegarMult(ram, cpu)); // pegarMult lê RAM[0]
    Soma(ram, cpu, 0, 1, 0);                               // RAM[0] = RAM[0] + RAM[1]

    resultado = pegarMult(ram, cpu); // pegarMult lê RAM[0]

    printf("O %d-esimo termo da PA e: %d\n", n, resultado);
}

void programaPotencia(RAM *ram, CPU *cpu, int base, int expoente)
{
    printf("Iniciando Calculo de Potencia: %d ^ %d\n", base, expoente);

    int resultado = 1;

    if (expoente == 0) // nesse caso a o resultado é sempre 1
    {
        reinicializarRAM(ram, 1);

        // RAM[0] = 1
        colocarNaRam(ram, cpu, 0, 1);
    }
    else
    {
        for (int i = 0; i < expoente; i++)
        {
            printf("Conta: (%d) * (%d)\n", resultado, base); // mostra o andamento da conta
            programaMult(ram, cpu, resultado, base);         // faz a conta
            resultado = pegarMult(ram, cpu);                 // armazena o resultado na variavel resultado
        }
    }

    printf("O resultado de %d ^ %d e: %d\n", base, expoente, resultado);
}

void programaFibonacci(RAM *ram, CPU *cpu, int n)
{
    printf("Iniciando Calculo de Fibonacci: F(%d)\n", n);

    reinicializarRAM(ram, 3);

    if (n == 0)
    {
        // pra pular o resto do código se n for 0
    }
    else if (n > 0) // quando n for 0, o resultado já é 0 e já está na RAM[0], quando for 1 RAM[0] = 1
    {
        // RAM[0] = 1
        colocarNaRam(ram, cpu, 0, 1);

        // Layout inicial para calcular:
        // RAM[0] (Atual)
        // RAM[1] (Anterior)
        if (n != 1) // soma a os números atuais e atualiza as posições
        {
            for (int i = 2; i <= n; i++) // loop que vai de 2 a n
            {
                Soma(ram, cpu, 0, 1, 2); // RAM[2] = RAM[0] + RAM[1]

                // Copiar RAM[0] -> RAM[1] (Atual -> Anterior)
                int temp_atual = pegarResultado(ram, cpu, 0);
                colocarNaRam(ram, cpu, 1, temp_atual);

                // Copiar RAM[2] -> RAM[0] (Soma_Temp -> Atual)
                int temp_soma = pegarResultado(ram, cpu, 2);
                colocarNaRam(ram, cpu, 0, temp_soma);
            }
        }
    }

    int resultado = pegarMult(ram, cpu); // lê RAM[0]

    printf("O resultado de Fibonacci F(%d) e: %d\n", n, resultado);
}

void programaMultMatriz(RAM *ram, CPU *cpu, int N)
{
    printf("Iniciando Multiplicacao de Matrizes %d x %d\n", N, N);

    // Criando e preenchendo matriz aleatoria

    int (*matrizA)[N] = malloc(sizeof(int[N][N]));
    int (*matrizB)[N] = malloc(sizeof(int[N][N]));

    for (int i = 0; i < N; i++) // Preenchendo
    {
        for (int j = 0; j < N; j++)
        {
            matrizA[i][j] = rand() % 10;
            matrizB[i][j] = rand() % 10;
        }
    }

    // Imprimindo matrizes
    printf("Matriz A (Gerada):\n");
    imprimirMatriz(N, N, matrizA);
    printf("Matriz B (Gerada):\n");
    imprimirMatriz(N, N, matrizB);

    int elementos = N * N;
    int tamanhoRAM = (elementos * 3) + 1; // define tamanho da ram
    int endereco_temp_produto = tamanhoRAM - 1;

    int matA = 0;
    int matB = elementos;
    int matC = 2 * elementos;

    reinicializarRAM(ram, tamanhoRAM);

    printf("Carregando Matriz A na RAM...\n");
    int endRam = matA; // endRam eh um endereço de ram
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            colocarNaRam(ram, cpu, endRam, matrizA[i][j]);
            endRam++; // aumenta um no endereço pra avançar a posicao
        }
    }

    printf("Carregando Matriz B na RAM...\n");
    endRam = matB;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            colocarNaRam(ram, cpu, endRam, matrizB[i][j]);
            endRam++;
        }
    }

    printf("Calculando C = A * B ...\n");
    // Três Loops de Cálculo

    // os calculos com dados foram feitos na maquina
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int end_C_ij = matC + (i * N) + j;
            for (int k = 0; k < N; k++)
            {
                int end_A_ik = matA + (i * N) + k;
                int end_B_kj = matB + (k * N) + j;

                multPosicoesRAM(ram, cpu, end_A_ik, end_B_kj, endereco_temp_produto);
                Soma(ram, cpu, end_C_ij, endereco_temp_produto, end_C_ij);
            }
        }
    }

    // Extrair Matriz C da RAM para imprimir
    int (*matrizC)[N] = malloc(sizeof(int[N][N]));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int end_C_ij = matC + (i * N) + j;
            matrizC[i][j] = pegarResultado(ram, cpu, end_C_ij);
        }
    }

    printf("Matriz C (Resultado):\n");
    imprimirMatriz(N, N, matrizC);

    // Liberando a memória alocada
    free(matrizA);
    free(matrizB);
    free(matrizC);
}

void programaIMC(RAM *ram, CPU *cpu, int peso, int altura) // usando apenas numeros inteiros
{
    printf("Iniciando Calculo de IMC\n");
    printf("Peso: %dkg, Altura: %dcm\n", peso, altura);

    // calculando numerador
    printf("Numerador: (%d * 10000)\n", peso);
    programaMult(ram, cpu, peso, 10000);

    // guardando na RAM
    int numerador = pegarMult(ram, cpu);
    printf("Numerador = %d\n", numerador);

    // calculando denominador
    printf("Denominador: (%d^2)\n", altura);
    programaPotencia(ram, cpu, altura, 2);

    // guardando na RAM
    int denominador = pegarMult(ram, cpu);
    printf("Denominador = %d\n", denominador);

    // Numerador/Denominador
    printf("Divisao: (%d / %d)\n", numerador, denominador);
    programaDiv(ram, cpu, numerador, denominador);

    // Obtendo resultado
    int imc = pegarDiv(ram, cpu);

    printf("O IMC (inteiro) eh: %d\n", imc);

    // tentar divisão
}

// Daniel

void programaMdc(RAM *ram, CPU *cpu, int a, int b)
{
    // SEPARAR O DE 0 A 4
    /*
    5 - 1
    6 - a
    7 - b
    8 - resultado(inicia com o menor deles)
    9 - menor deles
    */

    // Casos triviais
    // if (a == 0) return b;
    // if (b == 0) return a;

    reinicializarRAM(ram, 10);

    colocarNaRam(ram, cpu, 5, 1);

    colocarNaRam(ram, cpu, 6, a);

    colocarNaRam(ram, cpu, 7, b);

    // Euclides por subtrações
    while (pegarResultado(ram, cpu, 6) != pegarResultado(ram, cpu, 7))
    {
        // se a > b
        if (pegarResultado(ram, cpu, 6) > pegarResultado(ram, cpu, 7))
        {

            Subtrai(ram, cpu, 6, 7, 6);
            // a = a - b;
        }
        else
        {

            Subtrai(ram, cpu, 7, 6, 7);
            // b = b - a;
        }
    }
    // (= b)

    colocarNaRam(ram, cpu, 8, pegarResultado(ram, cpu, 6));

    // printf("Resultado do mdc: %d", pegarResultado(ram, cpu, 8));
}

void programaRestoDiv(RAM *ram, CPU *cpu, int n, int k)
{

    // Vou salvar na RAM 0

    reinicializarRAM(ram, 6);

    programaDiv(ram, cpu, n, k);

    int resto = pegarResultado(ram, cpu, 0);

    colocarNaRam(ram, cpu, 0, resto);

    // printf("O resto da divisao e: %d", resto);
}

void programaPG(RAM *ram, CPU *cpu, int firstValue, int razao, int numValues)
{

    /*
    5 - firstValue
    6 - razao
    7 - numValues
    8 - resultado
    */

    colocarNaRam(ram, cpu, 5, firstValue);
    colocarNaRam(ram, cpu, 6, razao);
    colocarNaRam(ram, cpu, 7, numValues);
    // colocarNaRam(ram, cpu, 8, firstValue);

    programaMultSemPrint(ram, cpu, pegarResultado(ram, cpu, 5), pegarResultado(ram, cpu, 6));
    for (int i = 0; i < pegarResultado(ram, cpu, 7); i++)
    {

        programaMultSemPrint(ram, cpu, pegarMult(ram, cpu), pegarResultado(ram, cpu, 6));
        colocarNaRam(ram, cpu, 8, pegarMult(ram, cpu));
        // printf("\n(Razão: %d * %d)", pegarResultado(ram, cpu, 5), pegarResultado(ram, cpu, 6));
    }

    printf("\nÚltimo valor da PG: %d", pegarResultado(ram, cpu, 8));
}

void programaLog(RAM *ram, CPU *cpu, int base, int valor)
{       /*
        0 - resultado exp
        5 - base
        6 - valor
        7 = chute
        8 - 1
        */

    reinicializarRAM(ram,10);




    colocarNaRam(ram, cpu, 5, base);
    colocarNaRam(ram, cpu, 6, valor);
    colocarNaRam(ram, cpu, 7, 1);
    colocarNaRam(ram, cpu, 8, 1);
    
    programaPotencia(ram, cpu, base, pegarResultado(ram, cpu, 7));
    Soma(ram, cpu, 7, 8, 7);


    while(pegarMult(ram,cpu) < pegarResultado(ram,cpu, 6)){

        programaPotencia(ram, cpu, base, pegarResultado(ram, cpu, 7));

        Soma(ram, cpu, 7, 8, 7);
    }
    
    printf("Resultado aproximado: %d", pegarResultado(ram, cpu, 7));


}

void determinante(RAM *ram, CPU, *cpu){

    /*

    Os números em frente a posição referem-se a pares que serão multiplicados
    e somados ou subtraídos para o cálculo do determinante.
    
    5 - ij(11) 1 2
    6 - ij(12) 2 3
    7 - ij(13) 3 1
    8 - ij(21) 3 3
    9 - ij(22) 1 1
    10 -ij(23) 2 2
    11 -ij(31) 2 1
    12 -ij(32) 3 2
    13 -ij(33) 1 3
    14 - ordem da matriz
    15 - resultado
    16 - soma das diagonais principais
    17 - soma das diagonais segundarias
    */



    reinicializarRAM(18);

    if(rand()%2 == 0){
        colocarNaRam(ram, cpu, 14, 2);
        colocarNaRam(ram, cpu, 5, (rand()%100));
        colocarNaRam(ram, cpu, 6, (rand()%100));
        colocarNaRam(ram, cpu, 8, (rand()%100));
        colocarNaRam(ram, cpu, 9, (rand()%100));
    }else{

        colocarNaRam(ram, cpu, 14, 3);

        colocarNaRam(ram, cpu, 5, (rand()%100));
        colocarNaRam(ram, cpu, 6, (rand()%100));
        colocarNaRam(ram, cpu, 8, (rand()%100));
        colocarNaRam(ram, cpu, 9, (rand()%100));
        colocarNaRam(ram, cpu, 7, (rand()%100));
        colocarNaRam(ram, cpu, 10, (rand()%100));
        colocarNaRam(ram, cpu, 11, (rand()%100));
        colocarNaRam(ram, cpu, 12, (rand()%100));
        colocarNaRam(ram, cpu, 13, (rand()%100));
    }


    if(pegarResultado(ram, cpu, 14) == 2){

        programaMult(ram, cpu, pegarResultado(ram, cpu, 5), pegarResultado(ram, cpu, 9));

        colocarNaRam(ram, cpu, 15, pegarMult(ram, cpu));
        programaMult(ram, cpu, pegarResultado(ram, cpu, 6), pegarResultado(ram, cpu, 8));
        Subtrai(ram, cpu, 15, 0, 15);

    }else if(pegarResultado(ram, cpu, 14) == 3){

        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 5), pegarResultado(ram, cpu, 9), pegarResultado(ram, cpu, 13));

        colocarNaRam(ram, cpu, 16, pegarMult(ram, cpu));
        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 6), pegarResultado(ram, cpu, 10), pegarResultado(ram, cpu, 11));
        Soma(ram, cpu, 16, 0, 16);
        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 7), pegarResultado(ram, cpu, 8), pegarResultado(ram, cpu, 12));
        Soma(ram, cpu, 16, 0, 16);    

        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 7), pegarResultado(ram, cpu, 9), pegarResultado(ram, cpu, 11));
        colocarNaRam(ram, cpu, 17, pegarMult(ram, cpu));
        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 5), pegarResultado(ram, cpu, 10), pegarResultado(ram, cpu, 12));
        Soma(ram, cpu, 17, 0, 17);
        programaMultTresValores(ram, cpu, pegarResultado(ram, cpu, 6), pegarResultado(ram, cpu, 8), pegarResultado(ram, cpu, 13));
        Soma(ram, cpu, 17, 0, 17);
        Subtrai(ram, cpu, 16, 17, 15);


    }

    colocarNaRam(ram, cpu, 0, pegarResultado(ram, cpu, 15));

    printf("O determinante e: %d", pegarResultado(ram, cpu, 0));




}